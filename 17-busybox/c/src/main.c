/* 
/ Title: 17-busybox
/
/ Description: explore all the things!
/
/ Author: PF
/
/ Created: 05sep2025
/ Last Updated:
/
/ Modinfo:
*/

#include <stdio.h>
#include <neo6502.h>
#include <neo/system.h>
#include <neo/graphics.h>

// local-API-supoprt files (copy them here)
// if these API updated get incorporated into the
// official llvm-mos-sdk repo, you would replace them with
// #include <neo/mouse.h>
// #include <neo/controller.h>
// #include <neo/math.h>
// (and remove {mouse, controller, math}.{c,h}, and also updating
// the Makefile to remove the .c files from the source list).
// Note: as of this writing, llvm-mos-sdk actually has an updated
// controller.h in main, but not in the releases.

#include "mouse.h"
#include "controller.h"
#include "math.h"

int main(void) {
  uint8_t i,st;

  st = neo_mouse_test_mouse_present();
  printf("Mouse present: %d\n",st);

  // thos doesn't seem to do anything on the emulator
  for(i=0;i<255;i++) { 
    neo_mouse_set_mouse_display_cursor(i);
    st = neo_api_error();
    printf("%d: neo_api_error() returned %d\n",i,st);
    uint32_t fc = neo_graphics_frame_count();
    while (neo_graphics_frame_count() < fc+30) /* wait*/ ;
    }
  return 0;

}
