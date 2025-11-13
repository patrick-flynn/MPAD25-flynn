/* 
/ Title: 25-randomredux
/
/ Author: PF
/
/ Created: 9/24/25
/
*/

#include <stdio.h>
#include <stdlib.h>
#include <neo6502.h>
#include <kernel.h>
#include <neo/console.h>

#include "math.h"

int main(void) {

  char *s = "1.0";
  float num = neo_math_string_to_number(s);
  char *ns = (char *)&num;

  printf("the string  1.0  became these fp bytes: %02x%02x%02x%02x\n",ns[0],ns[1],ns[2],ns[3]);

  }
