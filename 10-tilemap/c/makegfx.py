#!/usr/bin/env python

from PIL import Image
import struct

# this script converts a set of 64 1-bit tiles (in an 8x8 grid)
# in a tilesheet into
# a tileset.gfx file occupying the first 64 tile slots in the Neo652
# "graphics runtime"
#
# Note: the neo reads in column-major order!

sheetname = 'tileset_1bit.png'

gfx_header_fmt = '<BBBB'

NTILE=64
NSPRITE16=0
NSPRITE32=0

tileset = Image.open(sheetname).transpose(Image.Transpose.TRANSPOSE)
pixels = tileset.load()
w,h = tileset.size
assert (w == 8*16) and (h==8*16), f"Image size {tileset.size} not correct."

outpix = []

for tx in range(8):
  for ty in range(8):
    cropbox = (tx*16, ty*16, (tx+1)*16, (ty+1)*16)
    tile = tileset.crop(cropbox)
    tile.save(f'tile_{tx}_{ty}.png')
    for x in range(tx*16,tx*16+16):
      for y in range(ty*16, ty*16+16):
        pix = pixels[x,y]
        if pix == (0,0,0):
          outpix.append(0)
        elif pix == (255,255,255):
          outpix.append(7)
        else:
          print(f'Error: [{x},{y}]: {pix}')
          sys.exit(-1)

print(f'{len(outpix)} output pixels')
outbyte = []
for (p1,p2) in zip(outpix[::2],outpix[1::2]):  # :-P
  b = (p1<<4) | p2
  outbyte.append(b)

print(f'{len(outbyte)} output bytes')
gfx = struct.pack(gfx_header_fmt,1,NTILE,NSPRITE16,NSPRITE32)
pad = bytes([0]*252)


data = bytes(outbyte)

with open('tileset.gfx','wb') as f:
  f.write(gfx)
  f.write(pad)
  f.write(data)
