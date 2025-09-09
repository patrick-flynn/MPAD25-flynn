#!/usr/bin/env python

# PJF 7-Sep-2025
# make a 16x16 sprite

import struct

gfx_fmt = '<BBBB'

# layout of first four bytes: 0x01, #16x16tiles, #16x16sprites, #32x32sprites
gfx = struct.pack(gfx_fmt,1,0,1,0)
pad = bytes([0]*252)

data = [0]*(16*(16//2))

for i in range(16):
  for j in range(8):
    data[i*8+j] = (1+(i+j) % 15) * 0x11; # whee! Dup low nybble to high nybble

# make white border
for j in range(8):
  data[j] = data[15*8+j] = 0x77;
for i in range(16):
  data[i*8] = data[i*8+7] = 0x77;

# create a window in the middle of the 16x16 sprite with index 0 (transparent)

for row in range(5,11):
  for col in range(2,5):
    data[row*8+col] = 0

data = bytes(data)

with open('17sprite.gfx','wb') as f:
  f.write(gfx)
  f.write(pad)
  f.write(data)


