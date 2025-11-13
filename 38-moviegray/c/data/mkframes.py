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
			im3 = Image.fromarray(im2).resize((240,428)).crop((0,75,240,255))
			im4 = im3.convert('L')
			if (nf == 22):
				im4.save(f'{nf}.png',format='PNG')
			f.write(im4.tobytes())
			nf += 1
	
