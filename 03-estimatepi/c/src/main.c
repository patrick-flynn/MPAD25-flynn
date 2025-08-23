/* 
/ Title: 03-estimatepi
/
// Description: estimate pi using stochastic integration.
// Throw uniform random darts at the [-1,1] x [-1,1] square in 2d
// Determine the fraction that are alse inside the unit radius circle centered
// at the origin. The fraction of the latter to the total is pi/4.  Report
// the estimate of pi from those counts. 
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
float neo_math_random_decimal(void)
{
  // a union type to represent the math API's "register" construct
  union { char flag; float f; } register1;
  uint16_t r1ptr = (uint16_t)&register1;

  register1.flag = 1<<6; // specify that register1 will hold a float.
			 //
  // set up register1 to hold return value (we're not passing anything)
  ControlPort.params[0] = (uint8_t)(r1ptr & 0xFF);
  ControlPort.params[1] = (uint8_t)(r1ptr >> 8);
  ControlPort.params[2] = 0x01; // only one register in use

  KSendMessageSync(API_GROUP_MATH,API_FN_RND_DEC);
  // copy bytes of result (IEEE 32-bit float) from reg (on the stack frame)
  // into function result (which is preserved by the compiler)
  return register1.f;
}

int main(void) {
  unsigned int n_inside_circle = 0;
  unsigned int i;

  for(i=1;i<32700; i++) {
	  float x = neo_math_random_decimal();
	  float y = neo_math_random_decimal();
	  float r2 = x*x + y*y;
	  if (r2 <= 1.0) n_inside_circle++;
          printf("%u / %u\r",4*n_inside_circle, i);
	  }
  exit(0);
}
