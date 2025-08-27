#!/usr/bin/env python

import struct
import secrets

gfx_fmt = '<BBBB'

gfx = struct.pack(gfx_fmt,1,0,0,1)
pad = bytes([0]*252)
data = list(secrets.token_bytes(32*16)) # "each byte specifies 2 pixels"

# create a window in the middle of the 32x32 sprite with index 0 (transparent)

for row in range(10,22):
  for col in range(5,11):
    data[row*16+col] = 0

data = bytes(data)

with open('sprite.gfx','wb') as f:
  f.write(gfx)
  f.write(pad)
  f.write(data)


