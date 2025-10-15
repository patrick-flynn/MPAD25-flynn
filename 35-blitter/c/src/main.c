/* 
/ Title: 35-blitter
/
/ Description: Blitter test, again
/
/ Author: PF
/
/ Created: 10/15/25
/
*/

#include <stdio.h>
#include <stdlib.h>
#include <neo6502.h>
#include <kernel.h>
#include <neo/console.h>
#include <neo/graphics.h>
#include <neo/system.h>

#include "math.h"
#include "blitter.h"

#define SC_W 320
#define SC_H 240

void wait_ticks(uint16_t nticks) {
  while (nticks--) {
    uint32_t t = neo_system_timer();
    while (t == neo_system_timer()) ;
    }
  }


// a binary thang to blit 
unsigned char img[9600];
uint16_t w=SC_W,h=SC_H;

// display the thang at (x,y)
void disp(unsigned char *p, int16_t x, int16_t y, uint16_t w, uint16_t h) {
  blit_complex_rect rect_src = {p,0x0,0,w>>3,2,0,0,h,w};
  neo_blitter_image(NEO_BLITTER_ACTION_COPY,&rect_src,x,y,0);
  }
  
  

int main(void) {
  uint32_t i,j;
  uint16_t x,y,*p;

  neo_graphics_set_palette(1,255,255,255);

  while (1==1) {
    x = (w<SC_W) ? neo_math_random_integer(SC_W-w) : 0;  
    y = (h<SC_H) ? neo_math_random_integer(SC_H-h) : 0;  
    neo_console_set_cursor_pos(0,0);
    printf("%lu",i++);
    disp(img,x,y,w,h);
    
    // randomize the buffer
    p = (uint16_t *)img;
    for(j=0;j<9600/2;j++) *p++ = neo_math_random_integer(0xffff);
    while (1==1) ;
    }
  }
