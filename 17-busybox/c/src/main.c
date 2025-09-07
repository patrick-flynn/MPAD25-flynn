/* 
/ Title: 17-busybox
/
/ Description: explore all the things!
/
/ Author: PF
/
/ Created: 05sep2025
/ Last Updated:
/
/ Modinfo:
*/

#include <stdio.h>
#include <neo6502.h>
#include <neo/system.h>
#include <neo/graphics.h>

// local-API-support files (copy them here)
// if these API updated get incorporated into the
// official llvm-mos-sdk repo, you would replace them with
// #include <neo/mouse.h>
// #include <neo/controller.h>
// #include <neo/math.h>
// (and remove {mouse, controller, math}.{c,h}, and also updating
// the Makefile to remove the .c files from the source list).
// Note: as of this writing, llvm-mos-sdk actually has an updated
// controller.h in main, but not in the releases.

#include "mouse.h"
#include "controller.h"
#include "math.h"

uint8_t g_mouse=0, g_ncontrollers=0;

#define SC_W 320
#define SC_H 240

#define TRUE (1==1)
#define FALSE (1==0)

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

// get the dpad part of the controller report (low nybble of low byte)
#define DPAD(x) ((x)&0x0f)
// masks for the various directions
#define DP_L 0x01
#define DP_R 0x02
#define DP_U 0x04
#define DP_D 0x08

// get the YXBA part of the controller report (high nybble of low byte)
#define YXBA(x) (((x)>>4)&0x0f)
// masks for Y, X, B, A parts
// note: if a DS4 controller is used, Y is square and X is triangle
// and B is cross and A is circle
#define C_Y 0x08
#define SQ (C_Y)
#define C_X 0x04
#define TR (C_X)
#define C_B 0x02
#define CR (C_B)
#define C_A 0x01
#define CI (C_A)


void initialize(void) {
  uint8_t i;

  g_mouse = neo_mouse_test_mouse_present();
  printf("Mouse present: %d\n",g_mouse);

  neo_mouse_set_mouse_display_cursor(TRUE);

  g_ncontrollers = neo_controller_count();
  printf("Controllers present: %d\n",g_ncontrollers);

  neo_graphics_set_solid_flag(TRUE);

  return;
  }

int main(void) {
  initialize();
  while (1==1) {
    if ((neo_graphics_frame_count % 15) == 0) { // every 15th frame...
      // something graphically interesting
      uint16_t x1,y1,x2,y2,c,t;
      x1 = neo_math_random_integer(SC_W);
      x2 = neo_math_random_integer(SC_W);
      if (x1>x2) { t = x1; x1=x2; x2=t; };
      y1 = neo_math_random_integer(SC_H);
      y2 = neo_math_random_integer(SC_H);
      if (y1>y2) { t = y1; y1=y2; y2=t; };
      c = neo_math_random_integer(15) + 1;
      neo_graphics_set_color(c);
      neo_graphics_draw_rectangle(x1,y1,x2,y2);
      }

    uint32_t cs = neo_controller_read(g_ncontrollers);
    uint8_t x = cs&0xFF;
    uint8_t dpad = DPAD(x), abxy = ABXY(x);

    uint32_t fc = neo_graphics_frame_count();
    while (fc == neo_graphics_frame_count()) /* wait */ ;
    }
}
