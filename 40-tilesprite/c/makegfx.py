#!/usr/bin/env python

from PIL import Image
import struct
import operator
import functools

sheetname = 'Arcade - Bosconian - Player & Enemy Ships.png'

# this sheet is 116 columns, 176 pixels:
# One row of 7 16x16 sprites: six ships and a kaboom
# One row of three 32x32 kabooms 
# seven rows of seven 16x16 enemy ships each (different type per row)
# One row of three 16x16 kabooms 

tilesheet = 'tileset_1bit.png'
# the tilesheet is 128x128 and contains 64 16x16 tiles, 1 bit deep
# first, carve out the first four tiles.
NTILE=4
tileset = Image.open(tilesheet)
tilepix = []
for i in range(4):
  t = tileset.crop((i*16,0,(i+1)*16,16)).load()
  for ii in range(16):
    for jj in range(16):
      if t[ii,jj] == (0,0,0): tilepix.append(0)
      else: tilepix.append(7)

out_tilepix = []
for p1,p2 in zip(tilepix[::2],tilepix[1::2]):
  b = (p1<<4) | p2
  out_tilepix.append(b)

#process sprite

# this script rips every sprite out of the sheet, maps each color in the sprite to the "closest"
# color in the Neo6502 palette, and writes the result out in a gfx file. 

# read the spritesheet. 
spritesheet = Image.open(sheetname)
w,h = spritesheet.size
print(f'spritesheet is {spritesheet.size}')

sprite16 = []
sprite32 = []

# we're going to chop the sprites out of the sheet.
# looking at the sheet
# we'll specify a list of the upper-left corners and size of each sprite.
# start with the 16x16 ones
# there are seven 16x16 sprites in the original (untransposed) spreadsheet at rows
# 0, 3, 4, 5, 6, 7, 8, 9
# and three sprites in row 10
#
# we're counting in row numbers, and multiplying by 16 and 32 as we need to.
#
# Each cropped sprite is written to a PNG file so we can verify visually that 
# we cropped correctly.

for sprite16row in [0,3,4,5,6,7,8,9]: # the 16x16 sprites are on these rows
  for sprite16col in range(7):  # and in columns 0 through 6 inclusive
    # this is the cropping rectangle
    cropbox = [sprite16col*16, sprite16row*16, (sprite16col+1)*16, (sprite16row+1)*16]
    sprite = spritesheet.crop(cropbox)
    sprite.save(f'sprite_r{sprite16row}_c{sprite16col}.png')
    sprite16.append(sprite)

# Grab the last three sprites on row 10
for sprite16row in [10]:  # onoly one row (10) to process 
  for sprite16col in range(3): # columns 0, 1, 2
    # this is the cropping rectangle
    cropbox = [sprite16col*16, sprite16row*16, (sprite16col+1)*16, (sprite16row+1)*16]
    sprite = spritesheet.crop(cropbox)
    sprite.save(f'sprite16_r{sprite16row}_c{sprite16col}.png')
    sprite16.append(sprite)

# the sprite16s, in order, are
# six ships and a kaboom
# seven pinkships
# seven orangeships
# seven blueboomerangs
# seven pinkorangeboomerangs
# seven redmissiles
# seven greenmissiles
# seven yellowships
# three kabooms

# Assemble the sprite32s. Their row,column upper left corners are
# (0,16),(32,16),(64,16)
for sprite16row in [1]:  # the 32x32 sprite starts at row 16 (sprite16 row 1) 
  for sprite32col in range(3): # columns 0, 1, 2 in 32-pixel chunks
    # this is the cropping rectangle
    cropbox = [sprite32col*32, sprite16row*16, (sprite32col+1)*32, (sprite16row+2)*16]
    sprite = spritesheet.crop(cropbox)
    sprite.save(f'sprite32_r{sprite16row}_c{sprite32col}.png')
    sprite32.append(sprite)

# the sprite32s are all kabooms

# Now we loop over all the sprites and get the color index of each pixel in each sprite
# we're using the default palette, which uses colors from the venerable BBC Micro computer
# of the 1980s
# black(transparent),red, green, yellow, dark-blue, dark-purple, blue, white,
# black, dark-gray, dark-green, orange, brown, lavender, light-peach, light-gray)
# these numbers come from firmware/common/include/interface/palette.h in the firmware repo
palette = [(0, 0, 0), (255, 0, 77), (0, 228, 54), (255,236,39), (29,43,83), (126,37,83),
           (41,173,255), (255,241,232), (0,0,0), (95,87,79), (0,135,81), (255,163,0),
           (171,82,54), (131,118,156), (255,204,170), (194,195,199)]

# this function takes an (r,g,b) triple and gives the index (into the palette above)
# that best matches the triple (euclidean color distance)

def best_index(rgb): # rgb is a 3-tuple of red, green, blue intensity between 0 and 255
  bi = 0
  bd = sum((a-b)*(a-b) for a,b in zip(rgb,palette[0])) # initial condition
  for (i,tup) in enumerate(palette[1:]):
    d = sum((a-b)*(a-b) for a,b in zip(rgb,tup))
    if d<bd:
      bd=d
      bi=i
  return bi  

# process all the 16x16 sprites. Find the color indices of each pixel and put them on a list
# (one pixel per list element - we'll pack them into bytes later)
list16 = []
for s in sprite16:
  l = []
  sp = s.load() # actually get the pixels, PIL does lazy-loading
  for y in range(s.size[1]): # loop over every row
    for x in range(s.size[0]): # loop over every column
      l.append(best_index(sp[x,y]))
  list16.append(l)

# same for 32x32 sprites
list32 = []
for s in sprite32:
  l = []
  sp = s.load() # actually get the pixels, PIL does lazy-loading
  for y in range(s.size[1]): # loop over every row
    for x in range(s.size[0]): # loop over every column
      l.append(best_index(sp[x,y]))
  list32.append(l)

# now each 16x16 sprite is a list of 256 indices. Sprites are stored in gfx files with
# two adjacent pixels packed into one byte (since the indices are between 0 and 15
# inclusive, this saves half the storage and loses nothing). The sprite is stored in
# 128 bytes
# for 32x32, 1024 indices, 512 bytes

bytes16 = []
for l in list16:
  b16 = []
  for (idx1,idx2) in zip(l[::2],l[1::2]):  # tuples of adjacent bytes
    b16.append((idx1<<4)|idx2) # back two 4-bit indices into an 8-hit byte 
  bytes16.append(b16)

bytes32 = []
for l in list32:
  b32 = []
  for (idx1,idx2) in zip(l[::2],l[1::2]):  # tuples of adjacent bytes
    b32.append((idx1<<4)|idx2) # back two 4-bit indices into an 8-hit byte 
  bytes32.append(b32)

NTILE=4
NSPR16 = len(bytes16)
NSPR32 = len(bytes32)
print(f'{NSPR16} 16x16 sprites and {NSPR32} 32x32 sprites.')

gfx_header_fmt = '<BBBB'
pad = [[0]*252]

# create the header (4-byte thingy described on p. 27 of the Neo6502 documentation)
gfx = struct.pack(gfx_header_fmt,1,NTILE,NSPR16,NSPR32)
# pad it out to 256 bytes
pad = bytes([0]*252)

# write the gfx file. Header, padding, tiles (none here), 16x16 sprites, 32x32 sprites.
with open('tile+spriteset.gfx','wb') as f:
  f.write(gfx)
  f.write(pad)
  f.write(bytes(out_tilepix))
  for s16 in bytes16: f.write(bytes(s16))   # write each 16x16 sprite
  for s32 in bytes32: f.write(bytes(s32))   # write each 16x16 sprite


