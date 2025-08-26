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

/*******************/
unsigned long neo_math_random_integer(unsigned long max)
{
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


#define N_DOTS 512
#define X_SZ 320
#define Y_SZ 240

typedef struct {
 uint16_t x; // 0 - 319
 uint16_t y; // 0 - 239
 uint8_t color;
 } dot_t;

#define N_DOTS 512

int main(void) {
 dot_t dot[N_DOTS];
 uint16_t i;

 for(i=0;i<N_DOTS;i++) {
   dot[i].x = neo_math_random_integer(X_SZ);
   dot[i].y = neo_math_random_integer(Y_SZ);
   dot[i].color = neo_math_random_integer(256);
   neo_graphics_set_color(dot[i].color);
   neo_graphics_draw_pixel(dot[i].x,dot[i].y);
   }

   return 0;

}
