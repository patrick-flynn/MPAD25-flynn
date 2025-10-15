/* 
/ Title: 35-blitter
/
/ Description: Blitter test, again
/
/ Author: PF
/
/ Created: 10/15/25
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
#include "blitter.h"

#define SC_W 320
#define SC_H 240

void wait_ticks(uint16_t nticks) {
  while (nticks--) {
    uint32_t t = neo_system_timer();
    while (t == neo_system_timer()) ;
    }
  }


// a binary thang to blit - 16x16 target
char img[32] = {0xff, 0xff,
                0x80, 0x01,
                0x80, 0x01,
                0x80, 0x01,
                0x8f, 0xf1,
                0x8f, 0xf1,
                0x8f, 0xf1,
                0x8f, 0xf1,
                0x8f, 0xf1,
                0x8f, 0xf1,
                0x8f, 0xf1,
                0x8f, 0xf1,
                0x80, 0x01,
                0x80, 0x01,
                0x80, 0x01,
                0xff, 0xff };
  

MOOOO 

// display the thang at (x,y)
void disp(char *p, uint16_t x, uint16_t y) {
  blit_complex_rect rect_src = {p,0x0,0,2,2,0,1,16,16};
  neo_blitter_image(NEO_BLITTER_ACTION_COPY,&rect_src,0,0,0);
  }
  
  

int main(void) {
  uint8_t *ptr;
  uint8_t nn;
  uint16_t i,j;
  uint8_t im1,ip1,jm1,jp1;
  uint32_t t=0;

  board *board1=new_board(SC_H/2,SC_W/2,(uint8_t *)0x8000);
  board *board2=new_board(SC_H/2,SC_W/2,(uint8_t *)0xB000);
  board *curboard=board1,*nextboard=board2;
  board *tmpboard;

  // seed the random number generator
#if 0
  neo_console_set_cursor_pos(0,0);
  printf("Press any key.");
  while (!neo_console_read_char()) ; 
  neo_console_set_cursor_pos(0,0);
  uint16_t kmax = neo_system_timer() & 0x3ff;
  for(uint16_t k=0;k<kmax;k++) {
    t += neo_math_random_integer(0x7fffffff);
    //printf("%d of %d\n",k,kmax);
    };
#endif

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

  KWaitMessage();

  //neo_console_set_cursor_pos(0,0);
  //printf("Flargin");

  //wait_ticks(200);
  while (1==1) ;

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
