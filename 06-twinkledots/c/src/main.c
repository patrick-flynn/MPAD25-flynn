/* 
/ Title: 05-dots
/
/ Description: draw lotsa dots
/
/ Author: Flynn	
/
/ Created: 25-Aug-2025	
/ Last Updated:
/
/ Modinfo:
*/

#include <stdio.h>
#include <kernel.h>
#include <neo6502.h>
#include <neo/graphics.h>
#include <neo/system.h>

/*******************/
unsigned long neo_math_random_integer(unsigned long max) {
  typedef struct _neomathreg {
    uint8_t flag;
    union {
      unsigned long l;
      float f;
      } v;
    } neo_math_register;
  neo_math_register reg1,*reg1p=&reg1;

  // set max range of requested random integer... 5 bytes, every second address
  reg1.v.l = max;
  reg1.flag = 0x00; // integer
  ControlPort.params[0] = (uint8_t)((uint16_t)reg1p & 0x00ff);
  ControlPort.params[1] = (uint8_t)((uint16_t)reg1p >> 8) & 0x00ff;
  ControlPort.params[2] = 0x01; // only one reg, no interleaving

  KSendMessageSync(API_GROUP_MATH,API_FN_RND_INT);

  return reg1.v.l;
}
/*******************/


#define N_COLORS 16
#define N_DOTS 512
#define X_SZ 512
#define Y_SZ 512

typedef struct {
  uint16_t x; // 0 - 319
  uint16_t y; // 0 - 239
  uint8_t color;
  } dot_t;

int main(void) {
  dot_t dot[N_DOTS];
  uint16_t i;
  uint32_t j;
 
  // draw dots
  for(i=0;i<N_DOTS;i++) {
    uint16_t x = neo_math_random_integer(X_SZ);
    uint16_t y = neo_math_random_integer(Y_SZ);
    uint8_t ix = neo_math_random_integer(N_COLORS);
    dot[i].x = x;
    dot[i].y = y;
    dot[i].color = ix;
    neo_graphics_set_color(dot[i].color);
    neo_graphics_draw_pixel(dot[i].x,dot[i].y);
    }
 
  // twinkle the dots
  for(j=0;j<65535;j++) {
    for (i=0;i<N_DOTS;i++) {
      uint8_t ix;
      ix = neo_math_random_integer(N_COLORS);
      neo_graphics_set_color(ix);
      neo_graphics_draw_pixel(dot[i].x,dot[i].y);
      }
    }
   
  return 0;
 
 }
