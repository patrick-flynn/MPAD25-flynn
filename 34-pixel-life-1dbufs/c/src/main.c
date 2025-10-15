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
/ implementation of malloc, by telling the linker to ignmore memory above 0x8000
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
  uint8_t cur = GETPIX(b,r,c);
  uint8_t msk = (0x80 >> (c & 0x7));
  if (val) 
    b->base[(r)*((b->nc)>>3) + (c>>3)] |= msk; // set bit
  else
    b->base[(r)*((b->nc)>>3) + (c>>3)] &= ~msk; // set bit
  }

// display the board
// XXX this is SO SLOW
//void disp(board *b) {
  //neo_console_clear_screen();
  //for(uint16_t i=0;i<b->nr;i++) {
    //for(uint16_t j=0;j<b->nc;j++) {
      //uint8_t msk = (0x80) >> (j&0x7); // XXX
      //if GETPIX(b,i,j) neo_graphics_draw_pixel(i,j); // XXX XXX
      //}
    //}
//}

// display the board using the blitter - only works for 1D arrays
void disp(board *b) {
  blit_complex_rect rect_src = {b->base,0x0,0,(b->nc)>>3,2,0,1,b->nr,b->nc};
  neo_blitter_image(NEO_BLITTER_ACTION_COPY,&rect_src,0,0,0);
  }
  
  

int main(void) {
  uint8_t *ptr;
  uint8_t nn;
  uint16_t i,j;
  uint8_t im1,ip1,jm1,jp1;
  uint32_t t=0;

  board *board1=new_board(SC_H,SC_W,(uint8_t *)0x8000);
  board *board2=new_board(SC_H,SC_W,(uint8_t *)0xB000);
  board *curboard=board1,*nextboard=board2;
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

  neo_graphics_set_palette(1,255,255,255);


  // FOR DEBUGGING
  // clear the board
  //for(i=0;i<curboard->nr;i++) {
    //for(j=0;j<((curboard->nc) >> 3);j++) { // maybe unroll this
      //curboard->pix[i][j]=0;
      //}
    //}

  // plant a glider in the middle
  //setpix(curboard,SC_H/8,SC_W/8,1);
  //setpix(curboard,SC_H/8+1,SC_W/8+1,1);
  //setpix(curboard,SC_H/8+1,SC_W/8+2,1);
  //setpix(curboard,SC_H/8+2,SC_W/8,1);
  //setpix(curboard,SC_H/8+2,SC_W/8+1,1);

  // randomize board
  ptr = curboard->base;
  for(i=0;i<curboard->nr;i++) {
    for(j=0;j<((curboard->nc) >> 3);j++) { // implicitly unrolled
      *ptr++ = neo_math_random_integer(0x100);
      }
    }

  disp(curboard);

  neo_console_set_cursor_pos(0,0);
  printf("INITIAL BOARD");

  //while (1==1) ;

  while (1==1) { // XXX need a stop-detector
    for(i=0;i<curboard->nr;i++) {
      neo_console_set_cursor_pos(0,0);
      printf("row %d",i);
      im1 = (i-1) % curboard->nr;
      ip1 = (i+1) % curboard->nr;
      for(j=0;j<curboard->nc;j++) {
	    jm1 = (j-1) % curboard->nc;
	    jp1 = (j+1) % curboard->nc;
	    nn = GETPIX(curboard,im1,jm1) + GETPIX(curboard,im1,j) + GETPIX(curboard,im1,jp1) + 
	         GETPIX(curboard,i,jm1) + GETPIX(curboard,i,jp1) +
             GETPIX(curboard,ip1,jm1) + GETPIX(curboard,ip1,j) + GETPIX(curboard,ip1,jp1);
        //if (nn>0) {
          //printf("%d %d: nn %d live %d\n",i,j,nn,GETPIX(curboard,i,j));
          //};
        setpix(nextboard,i,j,rule(nn,GETPIX(curboard,i,j) ? 1 : 0));
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
