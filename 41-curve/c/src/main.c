/* 
/ Title: 41-curve
/
/ draw an approx. circular arc between two arbitrary points
/
/ Author: Flynn	
/
/ Created: 18-nov-2025
/ Last Updated:
/
/ Modinfo:
*/

#include <stdio.h>
#include <stdlib.h>
#include <kernel.h>
#include <neo6502.h>
#include <neo/graphics.h>
#include <neo/console.h>
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


#define SC_W 320
#define SC_H 240

typedef struct _pt {
 uint16_t x,y;
 } point_t;

#define NP 2

#define MIN(x,y) ((x)<(y) ? (x):(y))
#define MAX(x,y) ((x)>(y) ? (x):(y))
#define MID(x1,x2) (((x1)+(x2))/2)

int main(void) {
  while (1==1) {
    point_t point[NP]; 
    int16_t dx,dy,vx,vy;
    bool goodpts = false;
    while (!goodpts) { 
      // define vertices
      for(uint8_t i=0;i<NP;i++) {
        uint16_t x = SC_W/4 + neo_math_random_integer(SC_W/2);
        uint16_t y = SC_H/4 + neo_math_random_integer(SC_H/2);
        point[i].x = x;
        point[i].y = y;
        }
      dx = vx = point[1].x-point[0].x; dx = abs(dx);
      dy = vy = point[1].y-point[0].y; dy = abs(dy);
      if ((dx!=0) || (dy!=0)) goodpts = true;
      }

    // we have good points.
    int16_t xmin, xmax, ymin, ymax,xmid,ymid;
    xmin = MIN(point[0].x,point[1].x);
    xmax = MAX(point[0].x,point[1].x);
    ymin = MIN(point[0].y,point[1].y);
    ymax = MAX(point[0].y,point[1].y);
    ymid = MID(point[0].y,point[1].y);
    xmid = MID(point[0].x,point[1].x);

    neo_console_clear_screen();
    neo_graphics_set_color(1);
    neo_graphics_draw_ellipse(point[0].x-2,point[0].y-2,
                              point[0].x+2,point[0].y+2);
    neo_graphics_draw_ellipse(point[1].x-2,point[1].y-2,
                              point[1].x+2,point[1].y+2);
    neo_console_set_cursor_pos(0,0);
    printf("0:(%d %d) 1:(%d %d)\n",point[0].x,point[0].y,point[1].x,point[1].y);
    neo_graphics_set_color(2);
    neo_graphics_draw_line(point[0].x,point[0].y,point[1].x,point[1].y);

    // get maximal box by expanding dimension with smaller range
    // (symmetrically) to have the other dimension's size.
    if (dx>=dy) {
      ymin = MAX(0,ymid-dx/2);
      ymax = MIN(SC_H-1,ymid+dx/2);
      printf("xmM [%d %d] ymdM [%d %d %d]\n",xmin,xmax,ymin,ymid,ymax);
      }
    else { 
      xmin = MAX(0,xmid-dy/2);
      xmax = MIN(SC_W-1,xmid+dy/2);
      printf("xmdM [%d %d %d] ymM [%d %d]\n",xmin,xmid,xmax,ymin,ymax);
      }
    neo_graphics_draw_rectangle(xmin,ymin,xmax,ymax);

    // construct an approximate arc.
    // first get a point off of the segment but defined by it.
    uint16_t cx,cy;
    // special case: vertical segment: use "left box" centroid
    if (vx == 0) { 
      cx = point[0].x-(ymax-ymin)/4;
      cy = ymid;
      }
    else if (vy == 0) { //horizontal segment 
      cx = xmid;
      cy = point[0].y-(xmax-xmin)/4;
      }
    else { // tilted segment. Two cases. / (pos slope) and \ (neg slope)
      // using the barycenter of the triangle is a bit subtle,
      if ((vx*vy) > 0) { // case /
        printf("case /\n");
        cx = (7*xmax + xmin)/8; 
        cy = (7*ymin + ymax)/8;
        }
      else { // case '\' 
        printf("case \\\n");
        cx = (7*xmin + xmax)/8;
        cy = (7*ymin + ymax)/8;
        }
      }
    printf("c: [%d %d]\n",cx,cy);
    neo_graphics_set_color(3);
    neo_graphics_draw_ellipse(cx-2,cy-2,cx+2,cy+2);
    neo_graphics_set_color(5); // color 4 is REALLY DARK
    neo_graphics_draw_line(point[0].x,point[0].y,cx,cy);
    neo_graphics_draw_line(point[1].x,point[1].y,cx,cy);

    uint32_t fc = neo_graphics_frame_count();
    while (neo_console_read_char() == 0) /* wait */;
    //while (neo_graphics_frame_count() < fc+200 ) /* wait */ ;
    }
}
