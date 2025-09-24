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

typedef struct _neomathreg {
  uint8_t flag;
  union {
    unsigned long l;
    float f;
    } v;
  } neo_math_register;



unsigned long neo_math_random_integer(unsigned long max);
float neo_math_pstring_to_number(const neo_pstring_t *p_str);
float neo_math_string_to_number(const char *str);



#ifdef __cplusplus
}
#endif

#endif
