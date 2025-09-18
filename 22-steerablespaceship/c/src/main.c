/* 
/ Title: 22-steerablespaceship
/
/ Description: steer a spaceship around the screen
/
/ Author: Flynn	
/
/ Created: 29-Aug-2025	
/ Last Updated:
/
/ Modinfo:
*/

#include <stdio.h>
#include <stdlib.h>
#include <kernel.h>
#include <neo6502.h>
#include <neo/graphics.h>
#include <neo/system.h>
#include <neo/file.h>
#include <neo/sprites.h>
#include <neo/controller.h>
#include <neo/console.h>
#include "math.h"

// screen width (X) and height (Y)
#define SC_W 320
#define SC_H 240

// sprite width & height
#define SW 16
#define SH 16

#define GFX_FILE_NAME "spriteset.gfx"

// get the dpad part of the controller report (low nybble of low byte)
#define DPAD(x) ((x)&0x0f)
// masks for the various directions
#define DP_L 0x01
#define DP_R 0x02
#define DP_U 0x04
#define DP_D 0x08

#define MAX(x,y) ((x)>(y))?(x):(y)
#define MIN(x,y) ((x)<(y))?(x):(y)

// "position" of sprite in gfx (16x16 sprites start at 80; 32x32 at C0)
#define SPRITE_ID_BASE 0x80

void wait_frame(void) {
  // wait for the "number of frames drawn (i.e., screen updates)" to increment
  uint32_t t = neo_graphics_frame_count();
  while(t == neo_graphics_frame_count())  /* wait */ ;
  }


int main(int argc,char *argv[]) {
 uint16_t x=SC_W/2, y=SC_H/2; // pos of sprite
 uint8_t spid = 0; // the sprite to display.

  // load sprite
  neo_file_load(GFX_FILE_NAME,NEO_FILE_DESTINATION_GRAPHICS);

  while (1==1) {
	  // get controller state
	  uint32_t con = neo_controller_read(neo_controller_count());
	  uint8_t dpad = DPAD(con);
	  if (dpad != 0) {
		if (dpad & DP_L) x = MAX(x-1,SW/2);
	        if (dpad & DP_R) x = MIN(x+1,SC_W-SW/2);
	        if (dpad & DP_U) y = MAX(y-1,SH/2);
	        if (dpad & DP_D) y = MIN(y+1,SC_H-SH/2);	
                switch(dpad) {
                  case 0: break;
                  case 1: spid = 2; break;
                  case 2: spid = 6; break;
                  case 4: spid = 0; break;
                  case 5: spid = 1; break;
                  case 6: spid = 7; break;
                  case 8: spid = 4; break;
                  case 9: spid = 3; break;
                  case 0xa: spid=5; break;
                  default: {
                    printf("Error: invalid dpad value 0x%01xn",dpad);
                    exit(-1);
                    }
                  }
                }
          neo_sprite_set(1, x, y, ((SPRITE_ID_BASE+spid) & 0x7F),0,0);
          wait_frame();
          neo_console_set_cursor_pos(0,0);
          printf("spid: %02d",spid);
          if ((neo_graphics_frame_count() & 0x40) == 0)
           spid = spid ^ 0x8;
	}
}
