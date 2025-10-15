/* 
/ Title: 34-pixel-life-1dbufs
/
/ Description: Pixel-based Conway's Life
/
/ Author: PF
/
/ Created: 10/13/25
/
/ This version is based on 33-pixel-life but works around the rather limited
/ implementation of malloc, by telling the linker to ignore memory above 0x8000
/ and allocating the life buffers there as 1d items
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

  disp(curboard);

  while (1==1) { // XXX need a stop-detector
    for(i=0;i<curboard->nr;i++) {
      neo_console_set_cursor_pos(25,0);
      printf("row %d",i);
      im1 = (i-1) % curboard->nr;
      ip1 = (i+1) % curboard->nr;
      for(j=0;j<curboard->nc;j++) {
        uint8_t live = GETPIX(curboard,i,j);
	    jm1 = (j-1) % curboard->nc;
	    jp1 = (j+1) % curboard->nc;
	    nn = GETPIX(curboard,im1,jm1) + GETPIX(curboard,im1,j) + GETPIX(curboard,im1,jp1) + 
	         GETPIX(curboard,i,jm1) + GETPIX(curboard,i,jp1) +
             GETPIX(curboard,ip1,jm1) + GETPIX(curboard,ip1,j) + GETPIX(curboard,ip1,jp1);
        //if (nn>0) {
          //printf("%d %d: nn %d live %d\n",i,j,nn,live);
          //};
        setpix(nextboard,i,j,rule(nn,live) ? 1 : 0);
	    }
      }
    tmpboard = curboard;
    curboard = nextboard;
    nextboard = tmpboard;
    //while (1==1);
    disp(curboard);
    }
  return 0;
  }
