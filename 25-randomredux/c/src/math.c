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
  float f = *(float *)&l; 
  if (regs->type[0] == NEO_MATH_TYPE_FLOAT) 
    return f;
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

void nm_2regs_set_lval(neo_math_2registers *regs,uint8_t idx,int32_t l) {
  uint8_t *pl = (uint8_t *)&l; // whee!
  uint8_t *p = regs->contents+idx;
  *p = pl[0]; p+=2;
  *p = pl[1]; p+=2;
  *p = pl[2]; p+=2;
  *p   = pl[3];
  return;
  }
  


// all of the two-float-operand functions follow a very similar pattern.

#define NEO_MATH_2ARGS(funcname,funcnum) \
  float funcname(float f1,float f2) { \
    neo_math_2registers regs; \
    regs.type[0] = regs.type[1] = NEO_MATH_TYPE_FLOAT; \
    nm_2regs_set_fval(&regs,0,f1); \
    nm_2regs_set_fval(&regs,1,f2); \
    ((volatile uint16_t *)ControlPort.params)[0] = (uint16_t)&regs; \
    ControlPort.params[2] = 0x02;  \
    KSendMessageSync(API_GROUP_MATH,funcnum); \
    float res = nm_2regs_get_fval(&regs,0); \
    return res; \
    }
  
   
#if 0
float neo_math_2args(float f1, float f2,uint8_t funcnum) { 
  neo_math_2registers regs;
  regs.type[0] = regs.type[1] = NEO_MATH_TYPE_FLOAT; // force float args
  nm_2regs_set_fval(&regs,0,f1); 
  nm_2regs_set_fval(&regs,1,f2);
  ((volatile uint16_t **)ControlPort.params)[0] = (uint16_t *)&regs;
  ControlPort.params[2] = 0x02; // interleave = # of args
  KSendMessageSync(API_GROUP_MATH,funcnum);
  float res = nm_2regs_get_fval(&regs,0);
  return res;
  }
#endif

// one-float-argument functions
// also used for the (lone) no-argument function.
#define NEO_MATH_1ARG(funcname,funcnum) \
  float funcname(float f) { \
    neo_math_register reg1; \
    reg1.v.f = f; \
    reg1.type = NEO_TYPE_FLOAT; \
    ((volatile uint16_t *)ControlPort.params)[0] = (uint16_t)&reg1; \
    ControlPort.params[2] = 0x01; \
    KSendMessageSync(API_GROUP_MATH,funcnum); \
    return reg1.v.f; \
    }

NEO_MATH_2ARGS(neo_math_add,API_FN_ADD);
NEO_MATH_2ARGS(neo_math_sub,API_FN_SUB);
NEO_MATH_2ARGS(neo_math_mul,API_FN_MUL);
NEO_MATH_2ARGS(neo_math_div_dec,API_FN_DIV_DEC);
#if 0
// Function 0 : Addition
float neo_math_1arg(float f,uint8_t funcnum) {
  neo_math_register reg1;
  reg1.v.f = f;
  reg1.type = NEO_TYPE_FLOAT;
  ((volatile uint16_t *)ControlPort.params)[0] = (uint16_t)&reg1;
  ControlPort.params[2] = 0x01;
  KSendMessageSync(API_GROUP_MATH,funcnum);
  return reg1.v.f;
  }

// Function 0 : Addition
// Register1 := Register 1 + Register2

float neo_math_add(float f1, float f2) {
  return NEO_MATH_2ARGS(f1,f2,API_FN_ADD);
  }

// Function 1 : Subtraction
// Register1 := Register 1 - Register2

float neo_math_sub(float minuend, float subtrahend) {
  return NEO_MATH_2ARGS(minuend, subtrahend, API_FN_SUB);
  }

// Function 2 : Multiplication
//Register1 := Register 1 * Register2

float neo_math_mul(float f1, float f2) {
  return NEO_MATH_2ARGS(f1,f2,API_FN_MUL);
  }

// Function 3 : Decimal Division
// Register1 := Register 1 / Register2 (floating point)

float neo_math_div_dec(float dividend, float divisor) {
  return NEO_MATH_2ARGS(dividend, divisor, API_FN_DIV_DEC);
  }

// Function 4 : Integer Division
// Register1 := Register 1 / Register2 (integer result)
// not implemented

// Function 5 : Integer Modulus
// Register1 := Register 1 mod Register2
// not implemented

// Function 6 : Compare
// Parameter:0 := Register 1 compare Register2
//  returns $FF, 0, 1 for less equal and greater.
// Note: float comparison is approximate because of rounding.
// not implemented 

// Function 7 : Power
// Register1 := Register 1 to the power of Register2

float neo_math_pow(float base, float exponent) {
  return NEO_MATH_2ARGS(base, exponent, API_FN_POW);
  }

// Function 8 : Distance (counter-rectangle)
// Register1 := Square root of (Register1 * Register1) +
//                             (Register2 * Register2)
float neo_math_distance(float f1, float f2) {
  return NEO_MATH_2ARGS(f1, f2, API_FN_DISTANCE);
  }

// note: angle arguments aned return values are in
// degrees by default. Use neo_math_set_degree_radian_mode(b)
// (API function 35) to change this.
//  b is 0 for radians and nonzero for degrees
// Function 9 : Angle calculation (arctangent2)
// Register1 := arctangent2(Register 1,Register 2)
// 
float neo_math_atan2(float f1, float f2) {
  return NEO_MATH_2ARGS(f1,f2,API_FN_ATAN2);
  }

// Function 16 : Negate
// Register1 := -Register 1
float neo_math_negate(float f1) {
  return neo_math_1arg(f1,API_FN_NEG);
  }

// Function 17 : Floor
// Register1 := floor(Register 1)
float neo_math_floor(float f1) {
  return neo_math_1arg(f1,API_FN_FLOOR);
  }

// Function 18 : Square Root
// Register1 := square root(Register 1)
float neo_math_sqrt(float f1) {
  return neo_math_1arg(f1,API_FN_SQRT);
  }

// Function 19 : Sine
// Register1 := sine(Register 1) angles in degrees/radians
// see API function 35 to change mode 
float neo_math_sin(float f1) {
  return neo_math_1arg(f1,API_FN_SINE);
  }

// Function 20 : Cosine
// Register1 := cosine(Register 1) angles in degrees/radians
// see API function 35 to change mode 
float neo_math_cos(float f1) {
  return neo_math_1arg(f1,API_FN_COS);
  }


// Function 21 : Tangent
// Register1 := tangent(Register 1) angles in degrees/radians
// see API function 35 to change mode 
float neo_math_tan(float f1) {
  return neo_math_1arg(f1,API_FN_TAN);
  }

// Function 22 : Arctangent
// Register1 := arctangent(Register 1) angles in degrees/radians
// see API function 35 to change mode 
float neo_math_atan(float f1) {
  return neo_math_1arg(f1,API_FN_ATAN);
  }


// Function 23 : Exponent
// Register1 := e to the power of Register 1
float neo_math_exp(float f1) {
  return neo_math_1arg(f1,API_FN_EXP);
  }


// Function 24 : Logarithm
// Register1 := log(Register 1) natural logarithm, aka 
float neo_math_log(float f1) {
  return neo_math_1arg(f1,API_FN_LOG);
  }

// Function 25 : Absolute Value
// Register1 := absolute value(Register 1)
float neo_math_abs(float f1) {
  return neo_math_1arg(f1,API_FN_ABS);
  }

// Function 26 : Sign
// Register1 := sign(Register 1), returns -1 0 or 1
// not implemented

// Function 27 : Random  Decimal
// Register1 := random float from 0 - 1
// note - we use a dummy argument to avoid implementing
// a neo_math_1arg() helper for a single API call
// (we need 1 register for the result)
float neo_math_random_dec(void) {
  return neo_math_1arg(0.0,API_FN_RND_DEC);
  }
#endif

// Function 28 : Random Integer
// Register1 := random integer from 0 to (Register 1-1)
// needs custom implementation because it's an integer result
uint32_t neo_math_random_integer(uint32_t max) {
  neo_math_register reg1;
  reg1.v.l = max;
  reg1.type = NEO_MATH_TYPE_LONG; // integer
  ((volatile uint16_t *)ControlPort.params)[0] = (uint16_t)&reg1;
  ControlPort.params[2] = 0x01; // only one reg, no interleaving
  KSendMessageSync(API_GROUP_MATH,API_FN_RND_INT);
  return reg1.v.l;
}

// Function 32 : Number to Decimal
// Helper function for tokeniser, do not use.

// Function 33 : String to Number
// Convert the length prefixed string 
// at Parameters:4,5 to a constant in Register1.

// note: use neo_math_string_to_number(const char *str)
// when you have a C string (which is always)
 
// API call
float neo_math_pstring_to_number(const neo_pstring_t *p_str) {
  neo_math_register reg1;
  ((volatile uint16_t *)ControlPort.params)[0] = (uint16_t)&reg1;
  ControlPort.params[2] = 0x01; // only one register
  ((volatile uint16_t *)ControlPort.params)[4] = (uint16_t)p_str;
  KSendMessageSync(API_GROUP_MATH,API_FN_STR_TO_NUM);
  if (!(reg1.type & 0x40)) return 1.0 * reg1.v.l;
  else return reg1.v.f;
}

// glue code for C (null-terminated) strings
float neo_math_string_to_number(const char *str) {
  PASCALIZE_INPUT(str, str_p);
  return neo_math_pstring_to_number((neo_pstring_t *)str_p);
  }

// Function 34 : Number to String
// Convert the constant in Register1 to a length prefixed string which is stored at Parameters:4,5

// note: use neo_math_number_to_string(float f)
// when you want a C string (which is always)

void neo_math_number_to_pstring(float f,neo_pstring_t *p_str) {
  neo_math_register reg1;
  reg1.v.f = f;
  reg1.type = NEO_MATH_TYPE_FLOAT; // float
  // set up register1 -- no interleaving
  *((volatile uint16_t *)ControlPort.params) = (uint16_t)&reg1;
  ControlPort.params[2] = 0x01; // interleave (only one register)
  KSendMessageSync(API_GROUP_MATH,API_FN_NUM_TO_STR);
  // copy bytes of result to pstring address supplied as argument
  neo_pstring_t *r = (neo_pstring_t *)(ControlPort.params[4] | (ControlPort.params[5]<<8));
  for(uint16_t i=0;i<((char *)r)[0];i++) p_str[i] = r[i];
  return;
  } 

// glue code for C (null-terminated) strings
// note: str must be pre-allocated (max 256 bytes)
void neo_math_number_to_string(float f, char *str) {
  neo_math_number_to_pstring(f,(neo_pstring_t *)str);
  __neo_depascalize_output(str);
  return;
  }

#if 0
// Function 35 : Set Degree/Radian Mode
// Sets the use of degrees (the default) when non zero
// radians when zero.
void neo_math_set_deg_rad_mode(float f) {
  neo_math_1arg(f,API_FN_SET_DEG_RAD_MODE);
  return;
  }
#endif
