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
#include <neo/console.h>


uint8_t nr,nc;

uint8_t rule(uint8_t nn,uint8_t c) {
       return (c & ((nn==2)||(nn==3))) || (!c && (nn==3));
}

void disp(uint8_t **board) {
	uint8_t i,j;
	//neo_console_clear_screen();
	for(i=0;i<nr-1;i++) {
		for(j=0;j<nc;j++) {
			neo_console_set_cursor_pos(j,i);
			neo_console_write_char(board[i][j]?'*':' ');
			//printf("%c",board[i][j]?'*':' ');
		}
		//printf("\n");
	}
}


int main(void) {
  uint8_t cnt;
  uint8_t i,j;
  uint8_t **board1,**board2,**curboard,**nextboard,**tmpboard;
  uint8_t nn;
  uint8_t im1,ip1,jm1,jp1;

  //neo_console_clear_screen();
  neo_console_screen_size(&nc,&nr);
  printf("Woo, screen is %dw x %dh\n",nc,nr);

  curboard = board1 = (uint8_t **)malloc(nr*sizeof(*board1));
  for (i=0;i<nr;i++)
	  board1[i] = (uint8_t *)malloc(nc*sizeof(**board1));

  nextboard = board2 = (uint8_t **)malloc(nr*sizeof(*board2));
  for (i=0;i<nr;i++)
	  board2[i] = (uint8_t *)malloc(nc*sizeof(**board2));

  // initialize
  for(i=0;i<nr;i++) {
	  for(j=0;j<nc;j++) {
	 	 curboard[i][j] = ((i+j*j)&0x01) ? 1 : 0;
		 }
	  }


  cnt=0;
  while (cnt < 10) {
  	disp(curboard);
	for(i=0;i<nr;i++) {
		  im1 = (i-1) % nr;
		  ip1 = (i+1) % nr;
		  for(j=0;j<nc;j++) {
			  jm1 = (j-1) % nc;
			  jp1 = (j+1) % nc;
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
