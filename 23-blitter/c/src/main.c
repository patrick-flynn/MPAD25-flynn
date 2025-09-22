/* 
 * Title: 23-imagebkgnd
 * Author: P Flynn
 * Date: 22-Sep-2025
 *
 * this file reads a palette from imgpalette.bin (240 rgb entries)
 * and 320x240 pixels (palette indices) from imgbytes.bin
 * and splats the background image onto the screen using the blitter.
 */

#include <stdio.h>
#include <neo/graphics.h>
#include <neo/console.h>
#include <neo/file.h>

#define SC_W 320
#define SC_H 240

#define IMG_H 150
#define IMG_W 150

#define TRUE (0==0)
#define FALSE (0==1)

uint8_t img_bytes[22500]; // 150x150 dome

typedef struct _c_blit_rect_data {
  uint16_t addr;
  uint8_t sec_page;
  uint8_t pad;
  uint16_t stride;
  } blit_rect_data_t;

blit_rect_data_t brdSrc = {img_bytes,, brdDst;

MOO
  
int main(void) {
	uint16_t i,j;

    neo_file_open(1,"imgpalette.bin",NEO_FILE_READ);
    for(i=0;i<240;i++) {
      uint8_t pe[3];
      neo_file_read(1,pe,3);
      neo_graphics_set_palette(i,pe[0],pe[1],pe[2]);
      }
    neo_file_close(1);

    neo_file_load("imgbytes.bin",img_bytes);

    neo_blit_complex_copy(0,img_bytes,
