/* 
/ Title: 36-pixlife-more
/
/ Description: Pixel-based Conway's Life
/
/ Author: PF
/
/ Created: 10/15/25
/
/ more optimization of Conway's life for the Neo.
/
*/

#include <stdio.h>
#include <stdlib.h>
#include <neo6502.h>
#include <kernel.h>
#include <neo/console.h>
#include <neo/graphics.h>
#include <neo/system.h>

#include "math.h"
#include "mouse.h"
#include "blitter.h"

#define SC_W 320
#define SC_H 240

void wait_ticks(uint16_t nticks) {
  while (nticks--) {
    uint32_t t = neo_system_timer();
    while (t == neo_system_timer()) ;
    }
  }


// the rule for life at a cell in the next generation, based on its
// state (alive/dead) in the current generation and the number of living
// neighbors it has
uint8_t rule(uint8_t nn,uint8_t c) {
       return (c && ((nn==2)||(nn==3))) || (!c && (nn==3));
}

typedef struct _board {
  uint16_t nr,nc; // nc is actual width in pixels
  uint8_t *base; // base address
  } board;

board *new_board(uint16_t nr, uint16_t nc, uint8_t *base) {
  board *b = (board *)malloc(sizeof(board));
  b->nr=nr;
  b->nc=nc;
  b->base = base;
  return b;
  }

// macro - get the value at pixel (zero or one)

#define GETPIX(b,r,c) (((b)->base[(r)*(((b)->nc) >>3) + ((c) >>3)] & (0x80 >> (c&0x07)))  ? 1:0)
 
// val is 1 or 0!
void setpix(board *b,uint16_t r, uint16_t c, uint8_t val) {
  uint8_t *p = b->base + (r*(b->nc)>>3) + (c>>3); 
  uint8_t msk = (0x80 >> (c & 0x7));
  if (val) 
    *p |= msk; // set bit
  else
    *p &= ~msk; // clear bit
  }

void printboard(board *b) { printf("%drx%dc@%p",b->nr,b->nc,b->base); }
// display the board using the blitter - only works for 1D arrays

void disp(board *b) {
  blit_complex_rect rect_src = {b->base,0x0, /* addr and "page" (00 means RAM) */
                                0, /* filler*/
                                (b->nc)>>3, // stride between rows in src
                                2, /* 8 1-bit pixels per byte */
                                0,0, /* color specs for non-copy modes */
                                b->nr,b->nc}; /* height and width in dst */

  neo_blitter_image(NEO_BLITTER_ACTION_COPY,&rect_src,0,0,0);
  neo_console_set_cursor_pos(0,0);
  printf("disp("); printboard(b); printf(")");
  }
  
/* forward */
void life(board *c,board *n); 


int main(void) {
  uint8_t *ptr;
  uint8_t nn;
  uint16_t i,j;
  uint8_t im1,ip1,jm1,jp1;
  uint32_t t=0;
  uint16_t h=SC_H/2, w=SC_W/2;

  board board1 = {h, w, (uint8_t*)0x8000};
  board board2 = {h, w, (uint8_t*)0xA580}; /* 0x8000 + 9600 */
  board *curboard=&board1,*nextboard=&board2;
  board *tmpboard;

  // seed the random number generator
  neo_console_set_cursor_pos(0,0);
  printf("Press any key.");
  while (!neo_console_read_char()) ; 
  neo_console_set_cursor_pos(0,0);
  uint16_t kmax = neo_system_timer() & 0x3ff;
  for(uint16_t k=0;k<kmax;k++) {
    t += neo_math_random_integer(0x7fffffff);
    //printf("%d of %d\n",k,kmax);
    };


  /* we can only blit colors 0 and 1 - make sure 1 is white */
  neo_graphics_set_palette(1,255,255,255);

  // randomize board
  ptr = curboard->base;
  for(i=0;i<curboard->nr;i++) {
    for(j=0;j<((curboard->nc) >> 3);j++) { // implicitly unrolled
      *ptr++ = neo_math_random_integer(0x100);
      }
    }


  while (1==1) { // XXX need a stop-detector
    disp(curboard);
    life(curboard,nextboard);
    tmpboard = curboard;
    curboard = nextboard;
    nextboard = tmpboard;
    disp(curboard);
    }
  return 0;
  }

#define BPTR(b,i,j) ((b)->base + (i)*(((b)->nc)>>3) + (j)>>3)

void life(board *curboard,board *nextboard) {
  uint8_t *pm1m1,*pm10,*pm1p1,*p0m1,*p00,*p0p1,*pp1m1,*pp10,*pp1p1;
  uint8_t live,nn,mskmp1m1,mskmp10, makmp1p1,
  uint8_t msk0m1, msk00, msk0p1;
  uint16_t i,j,nr=curboard->nr,nc=curboard->nc;

  /* set up pointers for (0,0) */
  pm1m1 = BPTR(nr-1,nc-1); pm10 = BPTR(nr-1,0); pm1p1 = pm10+1;
  p0m1 = BPTR(0,nc-1); p00 = BPTR(0,0); p01 = p00+1;
  pp1m1 = BPTR(1,nc-1); pp10 = BPTR(1,0); pp1p1 = pp10+1;

  /* set up initial masks
  mskmp1m1 = 0x01; mskmp10 = 0xC0; mskmp1p1=0x80;
  msk0m1 = 0x01; msk00 = 0x40; msk0p1 = 0x80; 

  for(i=0;i<nr;i++) {:
