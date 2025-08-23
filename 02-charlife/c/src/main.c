/* 
/ Title: 02-charlife
/
/ Description: Character based Conway's Life
/
/ Author: PF
/
/ Created: 8/13/25
/
*/

#include <stdio.h>
#include <stdlib.h>
#include <neo6502.h>
#include <kernel.h>
#include <neo/console.h>

// this code based on the NeoAPIRand16() function written by Gollan, 9/3/24
unsigned long neo_math_random_integer(unsigned long max)
{
  uint8_t reg[5]; // Register1
  uint16_t regp = (uint16_t)reg;
  unsigned long res;

  // set max range of requested random integer... 5 bytes, every second address
  reg[0] = 0x00;   // type: 32-bit integer, little endian
  reg[1] = max&0xff; // byte0 (lsb) of max range
  reg[2] = (max>>8)&0xff; //0x00;   // byte1 of max range
  reg[3] = (max>>16)&0xff; //0x01;   // byte2 of max range
  reg[4] = (max>>24)&0xff; //0x00;   // byte3 (msb) of max range

  // set up register1
  ControlPort.params[0] = (uint8_t)(regp & 0xFF);
  ControlPort.params[1] = (uint8_t)(regp >> 8);
  ControlPort.params[2] = 0x01;

  KSendMessageSync(API_GROUP_MATH,API_FN_RND_INT);

  res =  (reg[1]&0xff) | (reg[2]<<8) | ((unsigned long)reg[3] <<16) | ((unsigned long)reg[4]<<24) ; 
  //printf("res %lx\n",res);
  return res;
}

// screen size - note: the board size is nr x (nc-1) to avoid scrolling resulting
// from character I/O in the righmost position of a line (no way to poke screen memory
// directly)
uint8_t nr,nc;

// board size
uint8_t br,bc;

// the rule for life at a cell in the next generation, based on its
// state (alive/dead) in the current generation and the number of living
// neighbors it has
uint8_t rule(uint8_t nn,uint8_t c) {
       return (c & ((nn==2)||(nn==3))) || (!c && (nn==3));
}

// display the board
void disp(uint8_t **board,uint8_t rows, uint8_t cols) {
	uint8_t i,j;
	neo_console_clear_screen();
	for(i=0;i<rows;i++) {
		for(j=0;j<cols;j++) {
			neo_console_set_cursor_pos(j,i);
			neo_console_write_char(board[i][j]?'*':' ');
		}
	}
}

uint8_t **makeboard(uint8_t rows, uint8_t cols) {
  uint8_t i;
  uint8_t **b = (uint8_t **)malloc(rows*sizeof(*b));
  if (b == NULL) {
	  printf("board malloc(%d) failed.\n",rows*sizeof(*b));
	  exit(-1);
  }
  for (i=0;i<rows;i++) {
	  b[i] = (uint8_t *)malloc(cols*sizeof(**b));
	  if (b[i] == NULL) {
		  printf("row %d malloc(%d) failed.\n",i,cols*sizeof(**b));
		  exit(-1);
	  }
  }
  return b;
}


int main(void) {
  uint8_t cnt;
  uint8_t nn;
  uint8_t i,j;
  uint8_t im1,ip1,jm1,jp1;
  uint8_t **board1,**board2,**curboard,**nextboard,**tmpboard;

  //neo_console_clear_screen();
  neo_console_screen_size(&nc,&nr);
  // board size is screen size less the rightmnost column
  br = nr; bc = nc-1;
  // allocate boards (current and next generations) and pointers
  // (we need two bufffers but they exchange roles at each generation)
  curboard = board1 = makeboard(br,bc);
  nextboard = board2 = makeboard(br,bc);

  // initialize - no random numbers yet
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
