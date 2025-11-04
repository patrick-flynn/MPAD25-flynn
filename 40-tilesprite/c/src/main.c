/* 
/ Title: 40-tilesprite
/
/ Description: draw a tilemap and then
/ wiggle a bunch of sprites onto the screen
/
/ Author: Flynn	
/
/ Created: 4-Nov-2025	
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
#include <neo/console.h>
#include "math.h"

// screen width (X) and height (Y)
#define SC_W 320
#define SC_H 240

// for tiling
#define BW SC_W/16
#define BH SC_H/16

#define GFX_FILE_NAME "tile+spriteset.gfx"

#define MAX(x,y) ((x)>(y))?(x):(y)
#define MIN(x,y) ((x)<(y))?(x):(y)

// "position" of sprite in gfx (16x16 sprites start at 80; 32x32 at C0)
#define S16_BASE 0x80
#define S32_BASE 0xC0

void wait_frame(void) {
  // wait for the "number of frames drawn (i.e., screen updates)" to increment
  uint32_t t = neo_graphics_frame_count();
  while(t == neo_graphics_frame_count())  /* wait */ ;
  }

typedef struct _spr {
  uint8_t id; // sprite id; >=0xC0 if 32x32; <=0xC0 and >=0x80 if 16x16
  uint8_t sz; // 16 or 32 (redundant)
  uint16_t x,y; // coords of topleft corner of sprite
  int8_t vx,vy;
  } spr_disp;


void draw_sprite(spr_disp *p, uint8_t idx) {
  neo_sprite_set(idx, p[idx].x, p[idx].y, p[idx].id & 0x7F, 0, 0);
  }

void update_sprite(spr_disp *p, uint8_t idx) {
  int16_t newx = p[idx].x + p[idx].vx, newy = p[idx].y + p[idx].vy;
  if ((newx >=0) && (newx < SC_W-p[idx].sz))
    p[idx].x = newx;
  else
    p[idx].vx *= -1;

  if ((newy >=0) && (newy < SC_H-p[idx].sz))
    p[idx].y = newy;
  else
    p[idx].vy *= -1;
  }

// tile stuff
typedef uint8_t tile_id;
tile_id tilemap[3+BW*BH];
#define TILE(r,c) (tilemap[3+(r)*BW+(c)])

int main(int argc,char *argv[]) {

  neo_file_load(GFX_FILE_NAME,NEO_FILE_DESTINATION_GRAPHICS);
  // load sprites
  // apparently you can't figure out programmatically the number of 16x16 and 32x32 sprites.
  // well, you can, but you have to reopen the file and I am lazy, so I will just define
  // constants
  uint8_t ntile = 4;
  uint8_t nspr16 = 59; // 8 rows of 7 plus 3 more
  uint8_t nspr32 = 3; // 3 big kabooms
  uint8_t nspr = nspr16 + nspr32;
  spr_disp sd[nspr];
  for(uint8_t i=0;i<nspr16;i++) {
    sd[i].id = i+S16_BASE;
    sd[i].sz = 16;
    sd[i].x = neo_math_random_integer(SC_W-16); // can't spill off the screen
    sd[i].y = neo_math_random_integer(SC_H-16);
    sd[i].vx = 0;
    sd[i].vy = 0;
    while (sd[i].vx * sd[i].vy == 0) { sd[i].vx = -2 + neo_math_random_integer(5); sd[i].vy = -2 + neo_math_random_integer(5); }
    }
  for(uint8_t i=0; i<nspr32; i++) {
    sd[i].id = i+S32_BASE;
    sd[i].sz = 32;
    sd[i].x = neo_math_random_integer(SC_W-32); // can't spill off the screen
    sd[i].y = neo_math_random_integer(SC_H-32);
    sd[i].vx = 0;
    sd[i].vy = 0;
    while (sd[i].vx * sd[i].vy == 0) { sd[i].vx = -2 + neo_math_random_integer(5); sd[i].vy = -2 + neo_math_random_integer(5); }
    }
  
  // tilemap
  tilemap[0] = 0x01;
  tilemap[1] = BW;
  tilemap[2] = BH;

  for(uint8_t i=0;i<BH;i++) for(uint8_t j=0;j<BW;j++)
    TILE(i,j) = neo_math_random_integer(ntile);

  neo_console_clear_screen();

  neo_graphics_set_tilemap(tilemap,0,0);
  neo_graphics_draw_tilemap(0,0,SC_W,SC_H);

  while (1==1) {
    for(uint8_t i=0;i<nspr;i++) { draw_sprite(sd,i); update_sprite(sd,i); }
    uint16_t i = neo_math_random_integer(BH);
    uint16_t j = neo_math_random_integer(BW);
    uint16_t k = neo_math_random_integer(ntile);
    TILE(i,j) = k;
    neo_graphics_draw_tilemap(0,0,SC_W,SC_H);
    wait_frame();
    }
}
