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
/ uses the blitter, ooh
/ 8-bit batched updates using the 3x3 neighborhood of bytes 
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
uint8_t ad[8][2] = { {0,0}, {0,0}, {0,1}, {1,1}, {0,0},{0,0},{0,0},{0,0}};
#define RULE(nn,alive) ad[(nn)][(alive)]

//uint8_t rule(uint8_t nn,uint8_t c) {
       //return (c && ((nn==2)||(nn==3))) || (!c && (nn==3));
//}

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
  uint16_t h=SC_H, w=SC_W;

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
    //disp(curboard);
    }
  return 0;
  }

#define BPTR(b,i,j) ((b)->base + (i)*(((b)->nc)>>3) + ((j)>>3))

uint8_t n1b[] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,  /* 0000 xxxx */
                 1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,  /* 0001 xxxx */
                 1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,  /* 0010 xxxx */
                 2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,  /* 0011 xxxx */
                 1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,  /* 0100 xxxx */
                 2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,  /* 0101 xxxx */
                 2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,  /* 0110 xxxx */
                 3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,  /* 0111 xxxx */
                 1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,  /* 1000 xxxx */
                 2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,  /* 1001 xxxx */
                 1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,  /* 1010 xxxx */
                 3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,  /* 1011 xxxx */
                 1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,  /* 1100 xxxx */
                 3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,  /* 1101 xxxx */
                 3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,  /* 1110 xxxx */
                 4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8};  /* 1111 xxxx */

/* calculate the next generation for a current generation 3x3 byte context */
uint8_t calcng(uint8_t cm1m1, uint8_t cm10, uint8_t cm1p1,
               uint8_t c0m1, uint8_t c00, uint8_t c0p1,
               uint8_t cp1m1, uint8_t cp10, uint8_t cp1p1) {
  uint8_t res=0,msk=0x80;
  uint8_t nn,live,j;
  /* MSB */
  nn = n1b[cm1m1 & 0x01] + n1b[cm10 & 0xC0] +
       n1b[c0m1 & 0x01] + n1b[c00 & 0x40] +
       n1b[cp1m1 & 0x01] + n1b[cp10 & 0xC0];
  live = n1b[c00 & 0x80];
  if (RULE(nn,live)) res |= msk;
  else res &= ~msk;
  msk >>= 1;
  /* bit 6 */
  nn = n1b[cm10 & 0xE0] + n1b[c00 & 0xA0] + n1b[cp10 & 0xE0];
  live = n1b[c00 & 0x40];
  if (RULE(nn,live)) res |= msk;
  else res &= ~msk;
  msk >>= 1;
  /* bit 5 */
  nn = n1b[cm10 & 0x70] + n1b[c00 & 0x50] + n1b[cp10 & 0x70];
  live = n1b[c00 & 0x20];
  if (RULE(nn,live)) res |= msk;
  else res &= ~msk;
  msk >>= 1;
  /* bit 4 */
  nn = n1b[cm10 & 0x38] + n1b[c00 & 0x28] + n1b[cp10 & 0x38];
  live = n1b[c00 & 0x10];
  if (RULE(nn,live)) res |= msk;
  else res &= ~msk;
  msk >>= 1;
  /* bit 3 */
  nn = n1b[cm10 & 0x1C] + n1b[c00 & 0x14] + n1b[cp10 & 0x1C];
  live = n1b[c00 & 0x08];
  if (RULE(nn,live)) res |= msk;
  else res &= ~msk;
  msk >>= 1;
  /* bit 2 */
  nn = n1b[cm10 & 0x0E] + n1b[c00 & 0x0A] + n1b[cp10 & 0x0E];
  live = n1b[c00 & 0x04];
  if (RULE(nn,live)) res |= msk;
  else res &= ~msk;
  msk >>= 1;
  /* bit 1 */
  nn = n1b[cm10 & 0x07] + n1b[c00 & 0x05] + n1b[cp10 & 0x07];
  live = n1b[c00 & 0x02];
  if (RULE(nn,live)) res |= msk;
  else res &= ~msk;
  msk >>= 1;
  /* bit 0 */
  nn = n1b[cm10 & 0x03] + n1b[cm1p1& 0x80] +
       n1b[c00 & 0x02] + n1b[c0p1 & 0x80] +
       n1b[cp10 & 0x03] + n1b[cp1p1 & 0x80];
  live = n1b[c00 & 0x1];
  if (RULE(nn,live)) res |= msk;
  else res &= ~msk;
  return res;
  }
  
      


void life(board *curboard,board *nextboard) {
// byte level (not pixel level) pointers
#define CBPTR(i,j) (BPTR((curboard),(i),((j)<<3)))
#define NBPTR(i,j) (BPTR((nextboard),(i),((j)<<3)))
  uint16_t nr=curboard->nr,nc=curboard->nc;
  int16_t i,j; 

  for(i=0;i<nr;i++) {
    /* set up initial pointers for row i */
    // TODO special case for first and last row
    uint8_t *pm1m1 = CBPTR((i-1)%nr,(nc-1)), *pm10 = CBPTR((i-1)%nr,0),  *pm1p1 = pm10+1;
    uint8_t *p0m1  = CBPTR(i,(nc-1)),         *p00  = CBPTR(i,0),        *p0p1   = p00+1;
    uint8_t *pp1m1 = CBPTR((i+1)%nr,(nc-1)),  *pp10 = CBPTR((i+1)%nr,0), *pp1p1 = pp10+1;
    uint8_t cm1m1 = *pm1m1, cm10 = *pm10, cm1p1 = *pm1p1;
    uint8_t c0m1  = *p0m1,  c00  = *p00,  c0p1  = *p0p1;
    uint8_t cp1m1 = *pp1m1, cp10 = *pp10, cp1p1 = *pp1p1;

    for(j=0;j<nc>>3;j++) { // process the byte at j
      uint8_t ng = calcng(cm1m1, cm10, cm1p1,
                          c0m1,  c00,  c0p1,
                          cp1m1, cp10, cp1p1);
      //NBPTR((i),(j)) = ng;
      nextboard->base[i*(nc>>3)+j] = ng;
      pm1p1++;
      p0p1++;
      pp1p1++;
      if (j == (nc>>3) -1)  { // wrap
        pm1p1 -= nc>>3;
        p0p1 -= nc>>3;
        pp1p1 -= nc>>3;
        }
      cm1m1 = cm10; cm10 = cm1p1; cm1p1 = *pm1p1;
      c0m1  = c00;  c00  = c0p1;  c0p1  = *p0p1;
      cp1m1 = cp10; cp10 = cp1p1; cp1p1 = *pp1p1;
      } 
    }
  }
