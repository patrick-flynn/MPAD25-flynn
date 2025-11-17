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

// a "blob" of two interleaved registers, offset of 2

typedef struct __neomathregs {
  uint8_t type[2]; // reg1 type is type[0], reg2 type is type[1]
  uint8_t contents[8];
  } neo_math_2registers;

void nm_2regs_set_fval(neo_math_2registers *regs,uint8_t idx,float f);
float nm_2regs_get_fval(neo_math_2registers *regs,uint8_t idx);


float neo_math_add(float f1, float f2);
float neo_math_sub(float f1, float f2);
float neo_math_mul(float f1, float f2);
float neo_math_div_decimal(float f1, float f2);
unsigned long neo_math_random_integer(unsigned long max);
float neo_math_pstring_to_number(const neo_pstring_t *p_str);
float neo_math_string_to_number(const char *str);
void neo_math_number_to_pstring(float f,neo_pstring_t *p_str);
void neo_math_number_to_string(float t, char *str);

#ifdef __cplusplus
}
#endif

#endif
