#!/usr/bin/env python


from PIL import Image
import sys

img = Image.open(sys.argv[1]).crop([425,200,553,328]).quantize(colors=240)
b = img.tobytes()
p = bytes(img.getpalette())

with open('imgbytes.bin','wb') as f:
  f.write(b)

with open('imgpalette.bin','wb') as f:
  f.write(p)

