/* 
/ Title: 10-tilemap
/
/ Description: draw a tilemap
/
/ Author: Flynn	
/
/ Created: 26-Aug-2025	
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
#include <neo/console.h>
#include <neo/file.h>

// screen width (X) and height (Y)
#define SC_W 320
#define SC_H 240

// tile w, h
#define TW 16
#define TH 16

// number of different tiles
#define NTILE 64

// "board" width and height in tiles
#define BW (SC_W/TW)
#define BH (SC_H/TH)

#define GFX_FILE_NAME "tileset.gfx"

void wait_frame(void) {
  // wait for the "number of frames drawn (i.e., screen updates)" to increment
  uint32_t t = neo_graphics_frame_count();
  while(t == neo_graphics_frame_count())  /* wait */ ;
  }

typedef uint8_t tile_id;
// tilemaps have 3 preamble bytes: 0x01, width, height

tile_id tilemap[3+BW*BH];
#define TILE(row,col) (tilemap[3+(row)*BW+(col)])

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


int main(int argc,char *argv[]) {
  uint16_t i,j,k;
  uint8_t err;

  printf("SCWH %d %d TWH %d %d BWH %d %d\n",SC_W,SC_H,TW,TH,BW,BH);

  // load tileset
  neo_file_load(GFX_FILE_NAME,NEO_FILE_DESTINATION_GRAPHICS);

  err = neo_api_error();
  if (err) { printf("API error %d on neo_file_load()\n",err);
    exit(-1);
    }

  // set up tilemap preamble
  tilemap[0] = 0x01;
  tilemap[1] = BW;
  tilemap[2] = BH;

  // initialize tilemap (indices of tiles to display)
  for(i=0;i<BH;i++) {
    printf("%02x: ",i);
    for(j=0;j<BW;j++) { 
      TILE(i,j) = neo_math_random_integer(NTILE);
      printf("%02x " ,TILE(i,j));
      }
    printf("\n");
    }

  neo_graphics_set_tilemap(tilemap,0,0);

  while (1==1) {
    neo_graphics_draw_tilemap(0,0,SC_W,SC_H);
    k = neo_math_random_integer(NTILE);
    i = neo_math_random_integer(BH);
    j = neo_math_random_integer(BW);
    TILE(i,j) = k;
    wait_frame();
    }
}

