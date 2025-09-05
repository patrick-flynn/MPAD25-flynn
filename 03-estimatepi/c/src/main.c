/* 
/ Title: 03-estimatepi
/
// Description: estimate pi using stochastic integration.
//
//// The idea:
// Throw uniform random darts at the [-1,1] x [-1,1] square in 2d
// Determine the fraction that are alse inside the unit radius circle centered
// at the origin. The fraction of the latter to the total is pi/4.  Report
// the estimate of pi from those counts. 
//
// The reality: floating-point support in LLVM-MOS is not complete, so we have to do this in
// integers.
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


int main(void) {
  unsigned long n_inside_circle = 0;
  unsigned long i;

  for(i=1;i<25000; i++) {
	  unsigned long x = neo_math_random_integer(32768);
	  unsigned long y = neo_math_random_integer(32768);
	  if ((x>=32768) || (y>=32768)) {
		  printf("ERROR: x %lu y %lu, should be <32768.\n",x,y);
		  exit(0);
	  }
	  unsigned long r2 = x*x + y*y;
	  if (r2 <= 32768*32768) n_inside_circle++;
      if ((i%1000) == 0) printf("%lu / %lu\r",4*n_inside_circle, i);
  }
  printf("Approximate estimate *100000: %llu\n",(unsigned long long)400000*(unsigned long long)n_inside_circle/(unsigned long long)i);
  exit(0);
}
