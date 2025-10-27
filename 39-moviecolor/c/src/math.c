#include <kernel.h>
#include <neo6502.h>
#include "math.h"

/*******************/
unsigned long neo_math_random_integer(unsigned long max)
{
  neo_math_register reg1;
  // set max range of requested random integer... 5 bytes, every second address
  reg1.v.l = max;
  reg1.flag = 0x00; // integer
  ((volatile uint16_t *)ControlPort.params)[0] = (uint16_t)&reg1;
  ControlPort.params[2] = 0x01; // only one reg, no interleaving
  KSendMessageSync(API_GROUP_MATH,API_FN_RND_INT);
  return reg1.v.l;
}

#if 0
float neo_math_pstring_to_number(const neo_pstring_t *p_str) {
  neo_math_register reg1;

  // set up register1 -- no interleaving
  ((volatile uint16_t *)ControlPort.params)[0] = (uint16_t)&reg1;
  ControlPort.params[2] = 0x01;

  ((volatile uint16_t *)ControlPort.params+3)[0] = (uint16_t)p_str;

  KSendMessageSync(API_GROUP_MATH,API_FN_STR_TO_NUM);

  return reg1.v.f;
}

float neo_math_string_to_number(const char *str) {
  PASCALIZE_INPUT(str, str_p);
  return neo_math_pstring_to_number((neo_pstring_t *)str_p);
  }

#endif
