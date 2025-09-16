/*
   / Title: a2-starfield
   /
   / Description: Assignment 2 problem 1 - interactive starfield
   /
   / Author: Flynn
   /
   / Created: 25-Aug-2025
   / Last Updated:
   /
   / Modinfo:
   */

#include <kernel.h>
#include <neo/console.h>
#include <neo/graphics.h>
#include <neo/mouse.h>
#include <neo/system.h>
#include <neo6502.h>
#include <stdio.h>
#include <stdlib.h>

/*******************/
unsigned long neo_math_random_integer(unsigned long max) {
  typedef struct _neomathreg {
    uint8_t flag;
    union {
      unsigned long l;
      float f;
    } v;
  } neo_math_register;
  neo_math_register reg1, *reg1p = &reg1;

  // set max range of requested random integer... 5 bytes, every second address
  reg1.v.l = max;
  reg1.flag = 0x00; // integer
  ControlPort.params[0] = (uint8_t)((uint16_t)reg1p & 0x00ff);
  ControlPort.params[1] = (uint8_t)((uint16_t)reg1p >> 8) & 0x00ff;
  ControlPort.params[2] = 0x01; // only one reg, no interleaving

  KSendMessageSync(API_GROUP_MATH, API_FN_RND_INT);

  return reg1.v.l;
}

#define SC_W 320
#define SC_H 240

#define N_DOTS 512
#define X_SZ 320
#define Y_SZ 240

#define FALSE 0
#define TRUE 1

#define MAX(a,b) ((a)>(b)?(a):(b))
typedef struct {
  uint16_t x;     // 0 - 319
  uint16_t y;     // 0 - 239
  int16_t vx, vy; // vector for razzle-dazzle, negative ok
  uint8_t color;
} dot_t;


void draw_stars(dot_t *d,uint16_t nd) {
  uint16_t i;
  neo_console_clear_screen();
  neo_console_set_cursor_pos(0,0);
  neo_graphics_set_solid_flag(TRUE);
  printf("draw %d dots\n",nd);
  for (i = 0; i < nd; i++) {
    neo_graphics_set_color(d[i].color);
    neo_graphics_draw_rectangle(d[i].x, d[i].y,d[i].x+4,d[i].y+4);
    }
}

int main(void) {
  dot_t dot[N_DOTS];
  uint16_t ndots = 0;
  int16_t i;
  uint8_t wasDown=FALSE;

  // clear screen-------
  neo_console_clear_screen();

  if (!neo_mouse_test_mouse_present()) {
    printf("Error: mouse not present.\n");
    exit(-1);
  }

  neo_mouse_set_mouse_display_cursor(1);
  neo_mouse_move_display_cursor(SC_W / 2, SC_H / 2);


  neo_graphics_set_solid_flag(TRUE);
  while (1 == 1) {
    uint16_t x, y;
    uint8_t buttonState, scrollWheelState;

    /* wait frame update */
    uint32_t fc = neo_graphics_frame_count();
    while (fc == neo_graphics_frame_count()) /* wait */ ;

    neo_mouse_get_mouse_state(&x, &y, &buttonState, &scrollWheelState);
    neo_console_set_cursor_pos(0,0);
    printf("%08lx: mouse %d %d 0x%x 0x%x\n",neo_system_timer(),x,y,buttonState,scrollWheelState);

    if (buttonState == 0x00) {
      wasDown = FALSE;
      continue;
      };
    if (buttonState & 0x01) { // left button down, plant a star
      if (wasDown) continue;
      neo_console_set_cursor_pos(0,1);
      printf("click %d %d\n",x,y);
      // this button was down in the last trip through the loop. Skip.
      // a new click.
      wasDown=TRUE;
      // only proceed if we have space
      neo_console_set_cursor_pos(0,2);
      printf("new [%d]",ndots);
      if (ndots < N_DOTS) {
        neo_console_set_cursor_pos(0,3);
        printf("create");
        dot[ndots].x = x;
        dot[ndots].y = y;
        int c = 1+neo_math_random_integer(15);
        dot[ndots].color = c;
        neo_console_set_cursor_pos(0,4);
        printf("create %d at %d %d, color %d",ndots,x,y,c);
        ndots++;
        }
      draw_stars(dot,ndots);
    } else if (buttonState & 0x02) {
      neo_console_set_cursor_pos(0,3);
      printf("VROOOOM");
      uint8_t visible = TRUE;

      // calculate vectors (approximate because integer math)
      for (i = 0; i < ndots; i++) {
        dot[i].vx =  -4 + neo_math_random_integer(8);
        if (dot[i].vx == 0) dot[i].vx += 1;
        dot[i].vy =  -4 + neo_math_random_integer(8);
        if (dot[i].vy == 0) dot[i].vy += 1;
        };

      while (visible) {
        visible = FALSE;
        for (i = 0; i < ndots; i++) {
          int16_t newx = dot[i].x + dot[i].vx;
          int16_t newy = dot[i].y + dot[i].vy;
          if ((newx >= 0) && (newy >= 0) && (newx < SC_W) && (newy < SC_H)) {
            visible = TRUE;
            dot[i].x = newx;
            dot[i].y = newy;
            }
          else {
            dot[i].x = dot[i].y = -100;
            }
          }
        neo_console_clear_screen();
        draw_stars(dot,ndots);
        fc = neo_graphics_frame_count();
        while (fc ==neo_graphics_frame_count()) /* wait */ ;
        }
      ndots=0;
      neo_console_clear_screen();
      }
  }
}
