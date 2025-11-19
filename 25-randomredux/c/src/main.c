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

  char *s = "0.7071";
  float num = neo_math_string_to_number(s);
  char *ns = (char *)&num;
  char s2[32];

  printf("the string  %s  became these fp bytes: %02x%02x%02x%02x\n",s,ns[0],ns[1],ns[2],ns[3]);

  neo_math_number_to_string(num,s2);
  printf("The floating point number in those bytes became this string: %s\n",s2);

  float res = neo_math_add(num,num);
  printf("The sum of %s and itself is 0x%lx ",s2,*((uint32_t *)(void *)&res));
  char s3[32];
  neo_math_number_to_string(res,s3);
  printf("(%s)\n",s3);

  res = neo_math_sub(num,num);
  printf("The difference of %s and itself is 0x%lx ",s2,*((uint32_t *)(void *)&res));
  char s4[32];
  neo_math_number_to_string(res,s4);
  printf("(%s)\n",s4);

  res = neo_math_mul(num,num);
  printf("The product of %s and itself is 0x%lx ",s2,*((uint32_t *)(void *)&res));
  char s5[32];
  neo_math_number_to_string(res,s5);
  printf("(%s)\n",s5);

  res = neo_math_div_dec(num,num);
  printf("The ratio of %s and itself is 0x%lx ",s2,*((uint32_t *)(void *)&res));
  char s6[32];
  neo_math_number_to_string(res,s6);
  printf("(%s)\n",s6);

  }
