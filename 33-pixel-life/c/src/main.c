/* 
/ Title: 33-pixellife
/
/ Description: Pixel-based Conway's Life
/
/ Author: PF
/
/ Created: 10/13/25
/
*/

#include <stdio.h>
#include <stdlib.h>
#include <neo6502.h>
#include <kernel.h>
#include <neo/console.h>

#include "math.h"
#include "mouse.h"

// a 320x240 screen of individual dots requires 320x240/8 = 9600 bytes.
// this is small enough that we can double-buffer. That doesn't help us
// in this version, but eventually...

#define SC_W 320
#define SC_H 240

// the rule for life at a cell in the next generation, based on its
// state (alive/dead) in the current generation and the number of living
// neighbors it has
uint8_t rule(uint8_t nn,uint8_t c) {
       return (c & ((nn==2)||(nn==3))) || (!c && (nn==3));
}

typedef struct _board {
  int nr,nc;
  uint8_t **pix;
  } board;

board new_board(uint16_t nr, uint16_t nc) {
  uint16_t i;
  board *b = (board *)malloc(sizeof(board));
  if (!b) { printf("ERROR: malloc(board)\n"); exit(0); }
  for(uint16_t i=0;i<nr;i++) {
    b[i] = (uint8_t *)malloc(nc*sizeof(uint8_t));
    if (!b[i]) { printf("ERROR: malloc(board[%d])\n",i); exit(0); }
    }
  return b;
  }


// display the board
// XXX this is SO SLOW
void disp(board *b) {
  neo_console_clear_screen();
  for(uint16_t i=0;i<b->nr;i++) {
    for(uint16_t j=0;j<b->nc;j++) {
      uint8_t msk = 0x80>>(j%8); // XXX
      if (b->pix[i][j>>3]&msk) neo_graphics_set_pixel(i,j); // XXX XXX
      }
    }
}

// macro - get the value at pixel (zero or nonzero, not zero or one!) XXX
#define GETPIX(b,r,c) ((b)->pix[r][c>>3]&(0x80>>(c%3))) 

// val is 1 or 0!
void setpix(board *b,uint16_t r, uint16_t c, uint8_t val) {
  uint8_t cur = GETPIX(b,r,c);
  uint8_t msk = (0x80 >> (c%3));
  if (val) 
    b->pix[r][c>>3] |= msk; // set the bit
  else
    b->pix[r][c>>3] &= ~msk; // clear the bit
  }


MOO


int main(void) {
  uint8_t cnt;
  uint8_t nn;
  uint8_t i,j;
  uint8_t im1,ip1,jm1,jp1;
  uint32_t t=0;

  board *board1=new_board(SC_H,SC_W);
  board *board2=new_board(SC_H,SC_W);
  board *curboard=board1,*nextboard=board2;

  // seed the random number generator
  printf("Press any key.");
  while (!neo_console_read_char()) ; 
  for(uint16_t k=0;k<neo_system_get_timer()&1023;k++) t += neo_math_random_integer(0x7fffffff);

  for(i=0;i<nr;i++) {
	  for(j=0;j<nc;j++) {
		 // 0 or 1
	 	 curboard[i][j] = neo_math_random_integer((unsigned long)2);
		 }
	  }


  cnt=0;
  while (1==1) { // XXX need a stop-detector
  	disp(curboard,br,bc);
	for(i=0;i<br;i++) {
		  im1 = (i-1) % br;
		  ip1 = (i+1) % br;
		  for(j=0;j<bc;j++) {
			  jm1 = (j-1) % bc;
			  jp1 = (j+1) % bc;
			  nn = curboard[im1][jm1] + curboard[im1][j] + curboard[im1][jp1] +
				  curboard[i][jm1] + curboard[i][jp1] +
				  curboard[ip1][jm1] + curboard[ip1][j] + curboard[ip1][jp1];
			  nextboard[i][j] =  rule(nn,curboard[i][j]) ? 1 : 0;
		  }
	  }
	  tmpboard = curboard;
	  curboard = nextboard;
	  nextboard = tmpboard;
  }

   return 0;

}
