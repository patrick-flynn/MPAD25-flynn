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

float neo_math_pstring_to_number(const neo_pstring_t *p_str) {
  printf("pstring ptr %p\n",p_str);
  uint8_t l = ((char *)p_str)[0];
  printf("pstring len is %d\n",l);
  printf("str: ");
  for(int i=1;i<=l;i++) { printf("%c ",((char *)p_str)[i]); }
  neo_math_register reg1;
  reg1.flag = 0x40; // specify float; don't think this is needed

  printf("reg1 at %p\n",&reg1);
  // set up register1 -- no interleaving
  ((volatile uint16_t *)ControlPort.params)[0] = (uint16_t)&reg1;
  ControlPort.params[2] = 0x01;
  

  // param 3 is skipped. 16-bit alignment?
  ControlPort.params[3] = 0xee;

  ControlPort.params[4] = ((uint16_t)p_str) & 0xff;
  ControlPort.params[5] = ((uint16_t)p_str >> 8) & 0xff;
  //((volatile uint16_t *)ControlPort.params)[2] = (uint16_t)&p_str;

  for(int i=0;i<6;i++) printf("params[%d] = %d (0x%02x)\n",i,ControlPort.params[i],ControlPort.params[i]);

  KSendMessageSync(API_GROUP_MATH,API_FN_STR_TO_NUM);

  struct __ControlPort cp;
  memcpy(&cp,&ControlPort,sizeof(struct __ControlPort));
  printf("API Exit: cp CFES 0x%02x 0x%02x 0x%02x 0x%02x\n",cp.command,cp.function,cp.error, cp.status);
  for(int i=0;i<6;i++) printf("cpparams[%d] = %d (0x%02x)\n",i,cp.params[i],cp.params[i]);

  if (ControlPort.error) {
    printf("neo_math_pstring_to_number() failed.\n");
    };
  printf("API Exit: ControlPort error: 0x%02x\n",ControlPort.status);
  printf("API Exit: ControlPort status: 0x%02x\n",ControlPort.status);
  for(uint8_t i=0;i<8;i++) printf("params[%d]: 0x%02x\n",i,ControlPort.params[i]);
  printf("reg1: flag 0x%x v.f %f v.l 0x%lx\n",reg1.flag,reg1.v.f, reg1.v.l);

  return reg1.v.f;
}

float neo_math_string_to_number(const char *str) {
  PASCALIZE_INPUT(str, str_p);
  return neo_math_pstring_to_number((neo_pstring_t *)str_p);
  }
