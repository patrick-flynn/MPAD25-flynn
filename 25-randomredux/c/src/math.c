#include <kernel.h>
#include <neo6502.h>

#include <stdio.h>

#include "math.h"

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


float neo_math_random_decimal(void) {
  neo_math_register reg1;
  reg1.flag = 0x40; // float (not needed?)
  ((volatile uint16_t *)ControlPort.params)[0] = (uint16_t)&reg1;
  ControlPort.params[2] = 0x01; // only one reg, no interleaving
  KSendMessageSync(API_GROUP_MATH,API_FN_RND_DEC);
  return reg1.v.f;
  }
  

float neo_math_pstring_to_number(const neo_pstring_t *p_str) {
  neo_math_register reg1;
  // set up register1 -- no interleaving
  ((volatile uint16_t *)ControlPort.params)[0] = (uint16_t)&reg1;
  ControlPort.params[2] = 0x01; // interleave (only one register)
  // set up input argument
  ControlPort.params[4] = (uint16_t)p_str&0xff;
  ControlPort.params[5] = ((uint16_t)p_str>>8)&0xff;

  KSendMessageSync(API_GROUP_MATH,API_FN_STR_TO_NUM);

  if (!(reg1.flag & 0x40)) return 1.0 * reg1.v.l;
  else return reg1.v.f;
  //return pr2->v.f;
}

float neo_math_string_to_number(const char *str) {
  printf("nmstn input [%p]  %s\n",str,str);
  PASCALIZE_INPUT(str, str_p);
  printf("nmstn pstr [%p] len %d, 3 chard %c%c%c\n",str_p,str_p[0],str_p[1],str_p[2],str_p[3]);
  return neo_math_pstring_to_number((neo_pstring_t *)str_p);
  }

void neo_math_number_to_pstring(float f,neo_pstring_t *p_str) {
  neo_math_register reg1;
  reg1.v.f = f;
  reg1.flag = 0x40; // float
  // set up register1 -- no interleaving
  ((volatile uint16_t *)ControlPort.params)[0] = (uint16_t)&reg1;
  ControlPort.params[2] = 0x01; // interleave (only one register)
  KSendMessageSync(API_GROUP_MATH,API_FN_NUM_TO_STR);
  // copy bytes of result to pstring address supplied as argument
  neo_pstring_t *r = (neo_pstring_t *)(ControlPort.params[4] | (ControlPort.params[5]<<8));
  for(uint16_t i=0;i<((char *)r)[0];i++) p_str[i] = r[i];
  return;
  } 

void neo_math_number_to_string(float f, char *str) {
  neo_math_number_to_pstring(f,(neo_pstring_t *)str);
  __neo_depascalize_output(str);
  return;
  }
