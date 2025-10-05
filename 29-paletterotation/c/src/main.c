/* 
 * Title: 29-imagebkgnd
 * Author: P Flynn
 * Date: 5-Oct-2025
 *
 * this file reads a palette from imgpalette.bin (240 rgb entries)
 * and 320x240 pixels (palette indices) from imgbytes.bin
 * and creates a background image
 *
 * after a pause, it starts to rotate colors in the palette.
 */

#include <stdio.h>
#include <neo/graphics.h>
#include <neo/console.h>
#include <neo/file.h>
#include <neo/system.h>
#include "math.h"

#define SC_W 320
#define SC_H 240

#define TRUE (0==0)
#define FALSE (0==1)

void wait_timer(uint16_t nticks) {
  // wait some number of ticks of the 100Hz system timer
  uint32_t t = neo_system_timer(),t2;
  while (--nticks) {
    while ((t2=neo_system_timer()) == t) ;
    t=t2;
    }
  }

int main(void) {
	uint16_t i,j;

        neo_file_open(1,"imgpalette.bin",NEO_FILE_READ);
        for(i=0;i<240;i++) {
          uint8_t pe[3];
          neo_file_read(1,pe,3);
          neo_graphics_set_palette(i,pe[0],pe[1],pe[2]);
          }
        neo_file_close(1);

        neo_file_open(1,"imgbytes.bin",NEO_FILE_READ);
        for(i=0;i<SC_H;i++) {
          uint8_t p[SC_W];
          neo_file_read(1,&p,SC_W);
          for(j=0;j<SC_W;j++) neo_graphics_write_pixel(j,i,p[j]);
          }
        neo_file_close(1);

        // if you use stdio to print out debugging info, the
        // background color clobbers everything else
        while (1==1) {
          //printf("w\n");
          wait_timer(100); // one second
          // may not be needed in the modern era?
          //printf("f\n");
          uint32_t fc = neo_graphics_frame_count();
          while (fc == neo_graphics_frame_count()) ; 

          uint8_t idx = neo_math_random_integer(240);
          uint8_t r = neo_math_random_integer(256);
          uint8_t g = neo_math_random_integer(256);
          uint8_t b = neo_math_random_integer(256);
          //printf("%d: %d %d %d\n",idx,r,g,b);
          neo_graphics_set_palette(idx,r,g,b);
          };
        }
