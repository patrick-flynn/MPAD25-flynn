/* 
/ Title: 14-spaceship
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
#include <kernel.h>
#include <neo6502.h>
#include <neo/graphics.h>
#include <neo/system.h>
#include <neo/file.h>
#include <neo/sprites.h>
#include <neo/controller.h>

// screen width (X) and height (Y)
#define SC_W 320
#define SC_H 240

// sprite width & height
#define SW 32
#define SH 32

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

// "position" of sprite in gfx (32x32 sprites start at C0)
#define SPRITE_ID_BASE 0xC0

void wait_frame(void) {
  // wait for the "number of frames drawn (i.e., screen updates)" to increment
  uint32_t t = neo_graphics_frame_count();
  while(t == neo_graphics_frame_count())  /* wait */ ;
  }


int main(int argc,char *argv[]) {
 uint16_t x=SC_W/2, y=SC_H/2; // pos of sprite

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
          	neo_sprite_set(1, x, y, ((SPRITE_ID_BASE+1) & 0x7F),0,0);
          	wait_frame();
	  }
  }
}

