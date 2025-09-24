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
  uint32_t seed,sum=0;

  printf("Enter seed: ");
  scanf("%lu",&seed);
  printf("%lu\n",seed);

  for(uint32_t i=0; i<seed; i++) { sum += neo_math_random_integer(0xFFFF); }
  printf("sum: %lu\n",sum);

  for(uint8_t i=0;i<10;i++)
    printf("%lu ",neo_math_random_integer(10000));
  }
