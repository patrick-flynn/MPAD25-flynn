#!/usr/bin/env python

from PIL import Image
import struct

# this script creates a gfx file of 18 32x32 sprites from a sprite sheet
# (original at https://opengameart.org/content/spaceship-360). Although
# the spreitesheet contains 108 rotated versions of the ship, we only need
# the first 90 degrees of rotation because we can flip the sprited horizontally
# and vertically.

sheetname = 'ship360_32.png'

gfx_header_fmt = '<BBBB'

NTILE=0
NSPRITE16=0
NSPRITE32=18

tileset = Image.open(sheetname).transpose(Image.Transpose.TRANSPOSE)
pixels = tileset.load()
w,h = tileset.size
assert (h == 32*6) and (w==32*12), f"Image size {tileset.size} not correct."

SSIZE=32

outpix = []

for tx in range(3):
  for ty in range(6):
    cropbox = (tx*SSIZE, ty*SSIZE, (tx+1)*SSIZE, (ty+1)*SSIZE)
    tile = tileset.crop(cropbox)
    tile.save(f'tile_{tx}_{ty}.png')
    for x in range(tx*SSIZE,(tx+1)*SSIZE):
      for y in range(ty*SSIZE, (ty+1)*SSIZE):
        pix = pixels[x,y]
        #print(f'({x},{y}) = {pix}')
        if (pix == (0,0,0,0)) or (pix[3]<255):
          outpix.append(0)
        else:
          outpix.append(0x2) # red *shrug*

print(f'{len(outpix)} output pixels')
outbyte = []
for (p1,p2) in zip(outpix[::2],outpix[1::2]):  # :-P
  b = (p1<<4) | p2
  outbyte.append(b)

print(f'{len(outbyte)} output bytes')
gfx = struct.pack(gfx_header_fmt,1,NTILE,NSPRITE16,NSPRITE32)
pad = bytes([0]*252)


data = bytes(outbyte)

with open('spriteset.gfx','wb') as f:
  f.write(gfx)
  f.write(pad)
  f.write(data)
