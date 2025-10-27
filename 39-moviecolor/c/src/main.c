/* 
/ Title: 39-moviecolor
/
/ Description: Blitter test
/
/ Author: PF
/
/ Created: 10/27/25
/
*/

#include <stdio.h>
#include <stdlib.h>
#include <neo6502.h>
#include <kernel.h>
#include <neo/console.h>
#include <neo/graphics.h>
#include <neo/system.h>
#include <neo/file.h>

#include "blitter.h"

#define SC_W 320
#define SC_H 240


void wait_ticks(uint16_t nticks) {
  while (nticks--) {
    uint32_t t = neo_system_timer();
    while (t == neo_system_timer()) ;
    }
  }


// display the byte thing pointed to by p at (x,y)
void disp_8bit(unsigned char *p, int16_t x, int16_t y, uint16_t w, uint16_t h) {
  blit_complex_rect rect_src = {p,0x0,0,w,0,0,0,h,w};
  neo_blitter_image(NEO_BLITTER_ACTION_COPY,&rect_src,x,y,0);
  }

// 160x120
#define IMG_DATA_SIZE 19200
// 240 colors
#define PALETTE_SIZE 720

uint8_t frame[IMG_DATA_SIZE]; 
uint8_t palette[PALETTE_SIZE];

int main(void) {
  uint32_t i;
  uint8_t *p;
  
  neo_file_open(1,"movie.bin",NEO_FILE_READ);

  while (1==1) {
    // read the frame palette and the frame
    neo_file_read(1,palette,PALETTE_SIZE);
    neo_file_read(1,frame,IMG_DATA_SIZE);
    if (neo_file_eof(1)) {
      neo_file_seek(1,0);
      continue;
      }
    // wait for "blanking interval" to start (?)
    // note: hardware is 60fps; slow down to 30
    uint32_t fc = neo_graphics_frame_count();
    while (neo_graphics_frame_count() != (fc+2)) ;
    for(i=0,p=palette; i<256; i++) {
      neo_graphics_set_palette(i,p[0],p[1],p[2]);
      p += 3;
      }
    disp_8bit(frame,0,0,160,120);
    }
  }
