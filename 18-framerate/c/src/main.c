/* 
/ Title: 18-framerate
/
/ Description: displays 100Hz system timer and frame number. Boring.
/
/ Author: PF
/
/ Created: 14sep2025
/ Last Updated:
/
/ Modinfo:
*/

#include <stdio.h>
#include <neo6502.h>
#include <neo/system.h>
#include <neo/graphics.h>
#include <neo/console.h>

int main(int argc, char *argv[]) {

  while (1==1) {
    neo_console_set_cursor_pos(0,0);
    unsigned long t = neo_system_timer(), fc = neo_graphics_frame_count();
    unsigned long long fps = (100000 * fc)/t;
    printf("timer: %lu -- frame: %lu -- 100k x fps: %llu",t,fc,fps);
    while (fc == neo_graphics_frame_count()) /* wait */ ;
    }
  }
