// Copyright 2025 LLVM-MOS Project
// Licensed under the Apache License, Version 2.0 with LLVM Exceptions.
// See https://github.com/llvm-mos/llvm-mos-sdk/blob/main/LICENSE for license
// information.

#include <stdbool.h>
#include <stdint.h>

#ifndef _NEO_BLITTER_H
#define _NEO_BLITTER_H

#include <neo6502.h>

#ifdef __cplusplus
extern "C" {
#endif

// blitter functions (Group 12)
#define API_GROUP_BLITTER 0x0C
#define API_FN_BLITTER_BUSY 1
#define API_FN_BLITTER_SIMPLE_COPY 2
#define API_FN_BLITTER_COMPLEX_COPY 3
#define API_FN_BLITTER_IMAGE 4

// blitter data structures
typedef struct _blit_complex_rect {
  uint8_t page;
  uint8_t *addr;
  uint8_t pad; // MUST be zero
  uint16_t stride;
  uint8_t format;
  uint8_t src_transparent;
  uint8_t src_solid;
  uint8_t src_height;
  uint16_t src_width;
  } blit_complex_rect;

#define NEO_BLITTER_ACTION_COPY 0
#define NEO_BLITTER_ACTION_COPYMASKED 1
#define NEO_BLITTER_ACTION_SOLIDMASKED 2

/**
 * @brief Blitter busy.
 */
bool neo_blitter_busy(void);

/**
 * @brief Simple blit copy.
 */
void neo_blitter_simple_copy(uint8_t src_bank, uint8_t *src_addr,
                             uint8_t dst_bank, uint8_t *dst_addr,
                             uint16_t n_bytes);

/**
 * @brief Complex blit copy.
 */

void neo_blitter_complex_copy(uint8_t action, blit_complex_rect *src_rect,
                              blit_complex_rect *dst_rect);

/**
 * @brief blit image.
 */

void neo_blitter_image(uint8_t action, blit_complex_rect *src_rect,
                       int16_t x, int16_t y, uint8_t format);

#endif


