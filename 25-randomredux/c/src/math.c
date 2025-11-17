#include <kernel.h>
#include <neo6502.h>

#include <stdio.h>

#include "math.h"

// this implementation is not 100% complete.
// Several functions in the Neo's math API allow some or all of the input
// arguments to be either float32s or int32s. To simplify function naming:
// - all such calls are prototyped as taking float arguments and returning
//   a float. C takes care of promoting args and the code below takes care
//   of promoting the result. 

// getter/setter for values in the two interleaved registers.
// helper functions that stuff float arguments into the 2-arg structure.
// using zero-based indexing internally:
// idx is 0 for reg1 and 1 for reg2.

float nm_2regs_get_fval(neo_math_2registers *regs,uint8_t idx) {
  uint8_t *p = regs->contents + idx;
  uint32_t l = p[0] | (((uint32_t)p[2])<<8) | (((uint32_t)p[4])<<16) | (((uint32_t)p[6])<<24);
  float *f = (float *)&l; 
  if (regs->type[0] == NEO_MATH_TYPE_FLOAT) 
    return *f;
  else
    return 1.0*l;
  }

void nm_2regs_set_fval(neo_math_2registers *regs,uint8_t idx,float f) {
  uint8_t *pl = (uint8_t *)&f; // whee!
  uint8_t *p = regs->contents+idx;
  *p = pl[0]; p+=2;
  *p = pl[1]; p+=2;
  *p = pl[2]; p+=2;
  *p   = pl[3];
  return;
  }


int32_t nm_2regs_get_lval(neo_math_2registers *regs,uint8_t idx) {
  uint8_t *p = regs->contents + idx;
  uint32_t l = p[0] | (((uint32_t)p[2])<<8) | (((uint32_t)p[4])<<16) | (((uint32_t)p[6])<<24);
  return l;
  }

void nm_2regs_set_lval(neo_math_2registers *regs,uint8_t idx,int32_t l)
  uint8_t *pl = (uint8_t *)&l; // whee!
  uint8_t *p = regs->contents+idx;
  *p = pl[0]; p+=2;
  *p = pl[1]; p+=2;
  *p = pl[2]; p+=2;
  *p   = pl[3];
  return;
  }
  


// all of the two-float-operand functions follow a very similar pattern.

#define neo_math_2args(FUNCNAME,FUNCNUM)  \
  "float "FUNCNAME"(float f1, float f2) {" \
  "neo_math_2registers regs;" \
  "regs.type[0] = regs.type[1] = NEO_MATH_TYPE_FLOAT;" \
  "nm_2regs_set_fval(&regs,0,f1);" \
  "nm_2regs_set_fval(&regs,1,f2);" \
  "((volatile uint16_t *)ControlPort.params)[0] = (uint16_t)&regs;" \
  "ControlPort.params[2] = 0x02;" \
  "KSendMessageSync(API_GROUP_MATH,"FUNCNUM");" \
  "float res = nm_2regs_get_fval(&regs,0);" \
  "return res;" \
  "}"
 
#define neo_math_1arg(FUNCNAME,FUNCNUM) \
  "float "FUNCNAME"(float f) {" \
    "neo_math_register reg1;" \
    "((volatile uint16_t *)ControlPort.params)[0] = (uint16_t)&reg1;" \
    "ControlPort.params[2] = 0x01;" \
    "KSendMessageSync(API_GROUP_MATH,"FUNCNUM");" \
    "return reg1.v.f;" \
    "}"
  

neo_math_2args(neo_math_add,API_FN_ADD)
neo_math_2args(neo_math_sub,API_FN_SUB)
neo_math_2args(neo_math_mul,API_FN_MUL)
neo_math_2args(neo_math_div_dec,API_FN_DIV_DEC)
// not implemented: API_FN_DIV_INT
// not implemented: API_FN_MOD
// not impleemnted (yet): API_FN_COMP
neo_math_2args(neo_math_pow,API_FN_POW)
neo_math_2args(neo_math_distance,API_FN_DISTANCE)

neo_math_1arg(neo_math_random_decimal,API_FN_RND_DEC)

// neo_math_compare: not implemented

uint32_t long neo_math_random_integer(uint32_t max)
{
  neo_math_register reg1;
  // set max range of requested random integer... 5 bytes, every second address
  reg1.v.l = max;
  reg1.type = NEO_MATH_TYPE_LONG; // integer
  ((volatile uint16_t *)ControlPort.params)[0] = (uint16_t)&reg1;
  ControlPort.params[2] = 0x01; // only one reg, no interleaving
  KSendMessageSync(API_GROUP_MATH,API_FN_RND_INT);
  return reg1.v.l;
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

  if (!(reg1.type & 0x40)) return 1.0 * reg1.v.l;
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
  reg1.type = NEO_MATH_TYPE_FLOAT; // float
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

/*
#define API_GROUP_MATH          0x04
#define API_FN_ADD              0x00
#define API_FN_SUB              0x01
#define API_FN_MUL              0x02
#define API_FN_DIV_DEC          0x03
#define API_FN_DIV_INT          0x04
#define API_FN_MOD              0x05
#define API_FN_COMP             0x06
#define API_FN_POW              0x07
#define API_FN_DISTANCE         0x08
#define API_FN_ATAN2            0x09
#define API_FN_NEG              0x10
#define API_FN_FLOOR            0x11
#define API_FN_SQRT             0x12
#define API_FN_SINE             0x13
#define API_FN_COS              0x14
#define API_FN_TAN              0x15
#define API_FN_ATAN             0x16
#define API_FN_EXP              0x17
#define API_FN_LOG              0x18
#define API_FN_ABS              0x19
#define API_FN_SIGN             0x1A
#define API_FN_RND_DEC          0x1B
#define API_FN_RND_INT          0x1C
#define API_FN_INT_TO_DEC       0x20
#define API_FN_STR_TO_NUM       0x21
#define API_FN_NUM_TO_STR       0x22
#define API_FN_SET_DEG_RAD_MODE 0x23
*/
