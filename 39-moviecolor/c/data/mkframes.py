#!/usr/bin/env python3

import cv2
from PIL import Image,ImageOps
import os
import sys
import io

nf=0
cap = cv2.VideoCapture('Brian Kelly dances to Bobby Shmurda [r-ATh9Q6oe4].mp4')
with open('movie.bin','wb') as f:
	while cap.isOpened():
		ret,im = cap.read()
		if not ret:
			break
		else:
			# could probably do this in one step, but...
			im2 = cv2.cvtColor(im,cv2.COLOR_BGR2RGB)
			im3 = Image.fromarray(im2).resize((160,285)).crop((0,50,160,170))
			if (nf == 22):
				im3.save(f'{nf}.png',format='PNG')
			im4 = im3.quantize(colors=240)
			im4_b = im4.tobytes()
			p_b = im4.palette.tobytes()
			f.write(p_b)
			f.write(im4_b)
			nf += 1
print(f'{nf} frames written')
