#!/usr/bin/env python

from PIL import Image
import struct
import operator
import functools

sheetname = 'Arcade - Bosconian - Player & Enemy Ships.png'

# this sheet is 116 columns, 176 pixels:
# One row of 7 16x16 sprites: six ships and a kaboom
# One row of three 32x32 kabooms 
# seven rows of seven enemy ships each (different type per row)
# One row of three 16x16 kabooms 

gfx_header_fmt = '<BBBB'
pad = [[0]*252]

NTILE=0
NSPRITE16=16
NSPRITE32=0

tileset = Image.open(sheetname).transpose(Image.Transpose.TRANSPOSE)
pixels = tileset.load()
w,h = tileset.size
print(f'tileset is {tileset.size}')

# output sprite size
SSIZE=16

# input sprite size as multiple of SSIZE
SCALE=1

outpix = []

tiles = []

# starting with the corners of the desired sprites, clip them out.
for (ty,tx) in [[0,0],[1,0],[2,0],[3,0],[4,0],[5,0]]:
    cropbox = (tx*SSIZE*SCALE, ty*SSIZE*SCALE, (tx+1)*SSIZE*SCALE, (ty+1)*SSIZE*SCALE)
    tile = tileset.crop(cropbox)
    tile.save(f'tile_{tx}_{ty}.png')
    tiles.append(tile)

# ok, the tiles, in order, are
# - N (engines firing)
# - N
# - NW (firing)
# - NW
# - W (firing)
# - W
# from these, we make all of the others
# the specs below are a triplet [sprite-number, flipLR, flipUD]

specs = [[0,False, False],[2,False,False],[4,False,False],[2,True,False],
         [0,True,False],[2,True,True],[4,False,True],[2,False,True]]

fulltiles = []

# the engines-firing sprites, CCW in 45 degree steps starting at N
for s in specs:
    t = tiles[s[0]]
    if s[1]: t = t.transpose(Image.FLIP_LEFT_RIGHT)
    if s[2]: t = t.transpose(Image.FLIP_TOP_BOTTOM)
    fulltiles.append(t)

# the engines-off sprites, same orientations
for s in specs:
    t = tiles[s[0]+1]
    if s[1]: t = t.transpose(Image.FLIP_LEFT_RIGHT)
    if s[2]: t = t.transpose(Image.FLIP_TOP_BOTTOM)
    fulltiles.append(t)


def to_neotile(t):
  """convert the PIL image to a neo6502 tile with the default palette."""
  colormap = [[(0,0,0),0],[(255,0,0),1],[(151, 151, 151),9],[(222,222,222),15]]
  nybbles = []
  outbytes = []
  px = t.load()
  for x in range(t.size[0]):
    for y in range(t.size[1]):
      for m in colormap:
        if px[x,y] == m[0]:
          nybbles.append(m[1])
  es = t.size[0]*t.size[1] # lol, always constant
  assert len(nybbles) == es, f'Error: expected {es} nybbles, got {len(nybbles)}'
  # nybbles contains 4-bit ints: pack successive pairs into 8-bit ints.
  for (p1,p2) in zip(nybbles[::2],nybbles[1::2]):  # :-P
    outbytes.append((p1<<4)|p2)
  return outbytes

ob = []
for f in fulltiles:
  ob += to_neotile(f)

print(f'{len(ob)} output bytes')
gfx = struct.pack(gfx_header_fmt,1,NTILE,NSPRITE16,NSPRITE32)
pad = bytes([0]*252)
outbytes = bytes(ob)

with open('spriteset.gfx','wb') as f:
  f.write(gfx)
  f.write(pad)
  f.write(outbytes)
