// Copyright 2025 LLVM-MOS Project
// Licensed under the Apache License, Version 2.0 with LLVM Exceptions.
// See https://github.com/llvm-mos/llvm-mos-sdk/blob/main/LICENSE for license
// information.

#include <string.h>
#include <neo6502.h>
#include <kernel.h>
//#include <api-internal.h>
#include "blitter.h"



// API group 12, function 1
bool neo_blitter_busy(void) {
  KSendMessageSync(API_GROUP_BLITTER,API_FN_BLITTER_BUSY);
  return ControlPort.params[0];
  }

// API group 12, function 2
// check neo_api_error() after calling
void neo_blitter_simple_copy(uint8_t src_bank, uint8_t *src_addr,
                             uint8_t dst_bank, uint8_t *dst_addr,
                             uint16_t n_bytes) {
  ControlPort.params[0] = src_bank;
  *((volatile uint16_t *)(ControlPort.params+1)) = (uint16_t)src_addr;
  ControlPort.params[3] = dst_bank;
  *((volatile uint16_t *)(ControlPort.params+4)) = (uint16_t)dst_addr;
  *((volatile uint16_t *)(ControlPort.params+6)) = (uint16_t)n_bytes;
  KSendMessageSync(API_GROUP_BLITTER,API_FN_BLITTER_SIMPLE_COPY); 
  return;
  }


// API group 12, function 3
void neo_blitter_complex_copy(uint8_t action, blit_complex_rect *src_rect,
                              blit_complex_rect *dst_rect) {
  ControlPort.params[0] = action;
  *((volatile uint16_t *)(ControlPort.params+1)) = (uint16_t)src_rect;
  *((volatile uint16_t *)(&ControlPort.params+3)) =(uint16_t) dst_rect;
  KSendMessageSync(API_GROUP_BLITTER,API_FN_BLITTER_COMPLEX_COPY); 
  return;
  }

// API group 12, function 4
void neo_blitter_image(uint8_t action, blit_complex_rect *src_rect,
                       int16_t x, int16_t y, uint8_t format) {
  ControlPort.params[0] = action;
  *((volatile uint16_t *)(&ControlPort.params[1])) = (uint16_t)src_rect;
  *((volatile uint16_t *)(&ControlPort.params[3])) = (uint16_t)x;
  *((volatile uint16_t *)(&ControlPort.params[5])) = (uint16_t)y;
  ControlPort.params[7] = format;
  KSendMessageSync(API_GROUP_BLITTER,API_FN_BLITTER_IMAGE); 
  return;
  }

