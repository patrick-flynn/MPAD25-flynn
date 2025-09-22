// Copyright 2025 LLVM-MOS Project
// Licensed under the Apache License, Version 2.0 with LLVM Exceptions.
// See https://github.com/llvm-mos/llvm-mos-sdk/blob/main/LICENSE for license
// information.

#include <stdbool.h>
#include <stdint.h>

#ifndef _NEO_MATH_H
#define _NEO_MATH_H

#include <neo6502.h>

#ifdef __cplusplus
extern "C" {
#endif


unsigned long neo_math_random_integer(unsigned long max);


#ifdef __cplusplus
}
#endif

#endif
