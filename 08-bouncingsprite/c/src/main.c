/* 
/ Title: 08-bouncingimg
/
/ Description: draw a moving sprite
/
/ Author: Flynn	
/
/ Created: 26-Aug-2025	
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

// screen width (X) and height (Y)
#define SC_W 320
#define SC_H 240

// sprite width & height
#define SW 32
#define SH 32

// number of positions of the sprite along its path

#define STEPS 200

#define GFX_FILE_NAME "sprite.gfx"

// sprite numbers start at 1 (turtle owns 0)
#define SPRITE_NUMBER 1
// "position" of sprite in gfx (32x32 sprites start at C0)
#define SPRITE_ID 0xC0

void wait_frame(void) {
  // wait for the "number of frames drawn (i.e., screen updates)" to increment
  uint32_t t = neo_graphics_frame_count();
  while(t == neo_graphics_frame_count())  /* wait */ ;
  }

int main(int argc,char *argv[]) {
 uint16_t x,y;
  // load sprite from sprite.gfx - in storage
  neo_file_load(GFX_FILE_NAME,NEO_FILE_DESTINATION_GRAPHICS);

  // if the gfx file is formatted properly, there whould be one 32x32
  // sprite in it, and its ID is therefore hex C0
  // Image parameter is sprite 1d without high bit set 

  // infinite loop bouncing the sprite around the screen.
  // the sprite is 32x32 and anchored at the center. Make sure none of it
  // goes off of the screen.
  // within 6 pixels of the edges
  
  int16_t i;

  uint16_t xoff = SW/2, yoff=SH/2;

  while (1==1) {
    for(i=0;i<STEPS;i++) {
      uint16_t x = xoff + i*(SC_W-SW)/STEPS;
      uint16_t y = yoff + i*(SC_H-SH)/STEPS;
      neo_sprite_set(SPRITE_NUMBER, x, y, (SPRITE_ID & 0x7F),0,0);
      wait_frame();
      }
    for(i=STEPS;i>=0;i--) {
      uint16_t x = xoff + i*(SC_W-SW)/STEPS;
      uint16_t y = yoff + i*(SC_H-SH)/STEPS;
      neo_sprite_set(SPRITE_NUMBER, x, y, (SPRITE_ID & 0x7F),0,0);
      wait_frame();
      }
    }

}

