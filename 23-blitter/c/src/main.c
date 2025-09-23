/* 
 * Title: 23-imagebkgnd
 * Author: P Flynn
 * Date: 22-Sep-2025
 *
 * this file reads a palette from imgpalette.bin (240 rgb entries)
 * and 128x128 pixels (palette indices) from imgbytes.bin
 * and splats the background image onto the screen using the blitter.
 */

#include <stdio.h>
#include <neo/graphics.h>
#include <neo/console.h>
#include <neo/file.h>

#include "blitter.h"

#define SC_W 320
#define SC_H 240

#define IMG_H 128
#define IMG_W 128

#define TRUE (0==0)
#define FALSE (0==1)

uint8_t img_bytes[IMG_W*IMG_H];
  
int main(void) {
    uint16_t i,j;
    blit_complex_rect brdSrc = {0x00,img_bytes,0x00,IMG_W,0,0,0,IMG_H,IMG_W};
    neo_file_open(1,"imgpalette.bin",NEO_FILE_READ);
    for(i=0;i<240;i++) {
      uint8_t pe[3];
      neo_file_read(1,pe,3);
      neo_graphics_set_palette(i,pe[0],pe[1],pe[2]);
      }
    neo_file_close(1);

    neo_file_load("imgbytes.bin",img_bytes);

    neo_blitter_image(NEO_BLITTER_ACTION_COPY,&brdSrc,10,10,0);
   
    while (1==1);
}
