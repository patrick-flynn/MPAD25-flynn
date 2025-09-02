/* 
/ Title: 15-polyline
/
/ Description: draw a squirming polyline reminiscent of old-skool
/ screen savers
/
/ Author: Flynn	
/
/ Created: 2-Sep-2025	
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


#define N_VERTICES 16
#define SC_W 320
#define SC_H 240

typedef struct {
  uint16_t x; // 0 - 319
  uint16_t y; // 0 - 239
  } vertex_t;

void draw_polyline(vertex_t *vtx, uint16_t nvtx, uint8_t color) {
    uint16_t i;
    neo_graphics_set_color(color);
    for(i=0;i<nvtx;i++) {
      uint16_t nxt = (i+1) % nvtx;
      neo_graphics_draw_line(vtx[i].x,vtx[i].y,vtx[nxt].x,vtx[nxt].y);
      }
    }

// this useful function will pause until the screen is redrawn.
void wait_frame(void) {
  uint32_t fc = neo_graphics_frame_count();
  while (neo_graphics_frame_count() == fc) /* wait */ ;
  }

int main(void) {
  vertex_t vertex[N_VERTICES];
  uint16_t i;
  uint8_t color=1;
 
  // define vertices
  for(i=0;i<N_VERTICES;i++) {
    uint16_t x = neo_math_random_integer(SC_W);
    uint16_t y = neo_math_random_integer(SC_H);
    vertex[i].x = x;
    vertex[i].y = y;
    }
 
  //draw the polyline
  draw_polyline(vertex,N_VERTICES,color);
  wait_frame();

  // loop forever. In the loop:
  //   a. erase the old polyline
  //   b. update the polyline vertices
  //   c. draw the polyline
  while (1==1) {
    draw_polyline(vertex,N_VERTICES,0); // draw in black to erase
    for(i=0;i<N_VERTICES;i++) {
      uint32_t r = neo_math_random_integer(0xffff);
      if (r&0x8000) 
        vertex[i].x += r&0x3;
      else 
        vertex[i].x -= r&0x3;
      // squash to one indecipherable line
      vertex[i].y += ((r&0x4000)?-1:1) * ((r&0x0C)>>2);
      }
    draw_polyline(vertex,N_VERTICES,color);
    wait_frame();
    }
}
