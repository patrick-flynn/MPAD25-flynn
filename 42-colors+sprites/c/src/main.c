/* 
/ Title: 42-colors+sprites
/
/ Description: steer a spaceship around the screen
/  also: demonstrate how sprites use the color palette
/
/ Author: Flynn	
/
/ Created: 29-Aug-2025	
/ Last Updated: 20-Nov-2025
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
  // wait for the frame count to increment
  uint32_t t = neo_graphics_frame_count();
  while(t == neo_graphics_frame_count())  /* wait */ ;
  }

#define SQSZ 12

void colors(uint16_t numcolors) {
  neo_console_clear_screen();
  // hiding sprite before drawing background seems necessary. ??!@?
  neo_sprite_hide(1);
  //for (uint8_t ii=0;ii<10;ii++) wait_frame();
  neo_graphics_set_solid_flag(true);
  //neo_graphics_set_color(0);
  //neo_graphics_draw_rectangle(0,0,SC_W-1,SC_H-1);
  //for (uint8_t ii=0;ii<10;ii++) wait_frame();
  uint16_t colorindex = 0;
  for(uint16_t y = 0; y<SC_H; y += SC_H/16)
    for(uint16_t x = 0; x<SC_W; x += SC_W/16) {
      neo_graphics_set_color(colorindex);
      neo_graphics_draw_rectangle(x,y,x+SQSZ-2,y+SQSZ-2);
      colorindex++;
      colorindex %= numcolors;
      }
  printf("Background colors: 0 through %d",numcolors);
  }

void rand_pal(uint16_t ncolors) {
  uint16_t i,j;
  uint32_t rl;
  uint8_t r, g, b;

  for(i=0;i<256;i++) {
    rl = neo_math_random_integer(0x7fffffff);
    r = rl & 0xff;
    g = (rl >> 8) & 0xff;
    b = (rl >> 16) & 0xff;
    neo_graphics_set_palette(i,r,g,b);
    }
  }

int main(int argc,char *argv[]) {
  uint16_t x=SC_W/2, y=SC_H/2; // pos of sprite
  uint8_t spid = 0; // the sprite to display.
  uint16_t ncolors=16;

  // load sprites
  neo_file_load(GFX_FILE_NAME,NEO_FILE_DESTINATION_GRAPHICS);

  neo_console_clear_screen();
  // draw "sane" background colors using low 4 bits
  colors(ncolors);

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
                  default: { // can get nonphysical dpad encoding is using kbd
                    continue;
                    }
                  }
          }
        neo_sprite_set(1, x, y, ((SPRITE_ID_BASE+spid) & 0x7F),0,0);
        // flashy light
        if ((neo_graphics_frame_count() & 0x80) == 0)
         spid = spid ^ 0x8;

        char c = neo_console_read_char();
        if (c) {
          switch(c) {
            case '1': ncolors=16; colors(ncolors); break;
            case '2': ncolors=256; colors(ncolors); break;
            case 'r': rand_pal(ncolors); colors(ncolors);  break;
            case 'R': neo_graphics_reset_palette(); break;
            default: break;
            }
          }
        wait_frame();
	}
}
