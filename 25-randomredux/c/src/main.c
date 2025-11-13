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

  char *s = "402.0";
  float num = neo_math_string_to_number(s);
  char *ns = (char *)&num;
  char s2[256];

  printf("the string  %s  became these fp bytes: %02x%02x%02x%02x\n",s,ns[0],ns[1],ns[2],ns[3]);

  neo_math_number_to_string(num,s2);
  printf("The floating point number in those bytes became this string: %s\n",s2);

  }
