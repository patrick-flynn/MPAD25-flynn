// Copyright 2025 LLVM-MOS Project
// Licensed under the Apache License, Version 2.0 with LLVM Exceptions.
// See https://github.com/llvm-mos/llvm-mos-sdk/blob/main/LICENSE for license
// information.

#include <stdbool.h>
#include <stdint.h>

#ifndef _NEO_MATH_H
#define _NEO_MATH_H

#include <neo6502.h>
#include "api-internal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NEO_MATH_TYPE_FLOAT 0x40
#define NEO_MATH_TYPE_LONG 0x00

typedef struct _neomathreg {
  uint8_t type;
  union {
    unsigned long l;
    float f;
    } v;
  } neo_math_register;

// a blob of two interleaved registers, offset of 2

typedef struct __neomathregs {
  uint8_t type[2]; // reg1 type is type[0], reg2 type is type[1]
  uint8_t contents[8];
  } neo_math_2registers;

// getters and setters for the blob
float nm_2regs_get_fval(neo_math_2registers *regs,uint8_t idx);
void nm_2regs_set_fval(neo_math_2registers *regs,uint8_t idx,float f);

int32_t float nm_2regs_get_lval(neo_math_2registers *regs,uint8_t idx);
void nm_2regs_set_lval(neo_math_2registers *regs,uint8_t idx,int32_t f);


// utilities
// two-argument function
// XXX perhaps this should be a macro
float neo_math_2args(float f1, float f2,uint8_t funcnum);
 
// one-argument functions
// XXX perhaps this should be a macro
float neo_math_1arg(float f,uint8_t funcnum);

// API functions

// Function 0 : Addition
// Register1 := Register 1 + Register2

float neo_math_add(float f1, float f2);

// Function 1 : Subtraction
// Register1 := Register 1 - Register2

float neo_math_sub(float minuend, float subtrahend);

// Function 2 : Multiplication
//Register1 := Register 1 * Register2

float neo_math_mul(float f1, float f2);

// Function 3 : Decimal Division
// Register1 := Register 1 / Register2 (floating point)

float neo_math_div_dec(float dividend, float divisor);

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

float neo_math_pow(float base, float exponent);

// Function 8 : Distance (counter-rectangle)
// Register1 := Square root of (Register1 * Register1) +

float neo_math_distance(float f1, float f2);

// note: angle arguments aned return values are in
// degrees by default. Use neo_math_set_degree_radian_mode(b)
// (API function 35) to change this.
//  b is 0 for radians and nonzero for degrees
// Function 9 : Angle calculation (arctangent2)
// Register1 := arctangent2(Register 1,Register 2)

float neo_math_atan2(float f1, float f2);

// Function 16 : Negate
// Register1 := -Register 1

float neo_math_negate(float f1);

// Function 17 : Floor
// Register1 := floor(Register 1)

float neo_math_floor(float f1);

// Function 18 : Square Root
// Register1 := square root(Register 1)

float neo_math_sqrt(float f1);

// Function 19 : Sine
// Register1 := sine(Register 1) angles in degrees/radians
// see API function 35 to change mode 

float neo_math_sin(float f1);

// Function 20 : Cosine
// Register1 := cosine(Register 1) angles in degrees/radians
// see API function 35 to change mode 

float neo_math_cos(float f1);

// Function 21 : Tangent
// Register1 := tangent(Register 1) angles in degrees/radians
// see API function 35 to change mode 
float neo_math_tan(float f1);

// Function 22 : Arctangent
// Register1 := arctangent(Register 1) angles in degrees/radians
// see API function 35 to change mode 

float neo_math_atan(float f1);


// Function 23 : Exponent
// Register1 := e to the power of Register 1

float neo_math_exp(float f1);


// Function 24 : Logarithm
// Register1 := log(Register 1) natural logarithm, aka 

float neo_math_log(float f1);

// Function 25 : Absolute Value
// Register1 := absolute value(Register 1)

float neo_math_abs(float f1);

// Function 26 : Sign
// Register1 := sign(Register 1), returns -1 0 or 1
// not implemented

// Function 27 : Random  Decimal
// Register1 := random float from 0 - 1
// note - we use a dummy argument to avoid implementing
// a neo_math_1arg() helper for a single API call
// (we need 1 register for the result)

float neo_math_random_dec(void);

// Function 28 : Random Integer
// Register1 := random integer from 0 to (Register 1-1)

uint32_t long neo_math_random_integer(uint32_t max);

// Function 32 : Number to Decimal
// Helper function for tokeniser, do not use.

// Function 33 : String to Number
// Convert the length prefixed string 
// at Parameters:4,5 to a constant in Register1.
// note: use neo_math_string_to_number(const char *str)
// when you have a C string (which is always)
 
// API call
float neo_math_pstring_to_number(const neo_pstring_t *p_str);

// glue code for C (null-terminated) strings
float neo_math_string_to_number(const char *str);

// Function 34 : Number to String
// Convert the constant in Register1 to a length prefixed string which is stored at Parameters:4,5
// note: use neo_math_number_to_string(float f)
// when you want a C string (which is always)

void neo_math_number_to_pstring(float f,neo_pstring_t *p_str);

// glue code for C (null-terminated) strings
// note: str must be pre-allocated (max 256 bytes)
void neo_math_number_to_string(float f, char *str);

// Function 35 : Set Degree/Radian Mode
// Sets the use of degrees (the default) when non zero
// radians when zero.
void neo_math_set_deg_rad_mode(float f);

#ifdef __cplusplus
}
#endif

#endif
