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

#define VELOCITY_MAX 3

typedef struct {
  uint16_t x; // 0 - 319
  uint16_t y; // 0 - 239
  int8_t vx;
  int8_t vy;
  } vertex_t;

void draw_polyline(vertex_t *vtx, uint16_t nvtx, uint8_t color) {
    uint16_t i;
    neo_graphics_set_color(color);
    for(i=0;i<nvtx;i++) {
      uint16_t nxt = (i+1) % nvtx;
      neo_graphics_draw_line(vtx[i].x,vtx[i].y,vtx[nxt].x,vtx[nxt].y);
      }
    }

void update_polyline(vertex_t *vtx, uint16_t nvtx) {
    uint16_t i;
    int16_t newx,newy;
    for(i=0;i<nvtx;i++) {
      newx = (int16_t)vtx[i].x + vtx[i].vx;
      newy = (int16_t)vtx[i].y + vtx[i].vy;

      if (newx < 0) {
        vtx[i].x = 0;
        vtx[i].vx *= -1;
        } 
      else if (newx >= SC_W) {
        vtx[i].x = SC_W-1;
        vtx[i].vx *= -1;
        } 
      else
        vtx[i].x = newx;

      if (newy < 0) {
        vtx[i].y = 0;
        vtx[i].vy *= -1;
        }
      else if (newy >= SC_H) {
        vtx[i].y = SC_H-1;
        vtx[i].vy *= -1;
        }
      else 
        vtx[i].y = newy;
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
  uint8_t color= 1 + neo_math_random_integer(15);
 
  // define vertices
  for(i=0;i<N_VERTICES;i++) {
    uint16_t x = neo_math_random_integer(SC_W);
    uint16_t y = neo_math_random_integer(SC_H);
    vertex[i].x = x;
    vertex[i].y = y;
    vertex[i].vx = neo_math_random_integer(2*VELOCITY_MAX-1) - VELOCITY_MAX;
    vertex[i].vy = neo_math_random_integer(2*VELOCITY_MAX-1) - VELOCITY_MAX;
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
    update_polyline(vertex,N_VERTICES);
    draw_polyline(vertex,N_VERTICES,color);
    color = 1 + neo_math_random_integer(15);
    wait_frame();
    }
}
