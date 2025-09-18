/* 
 * Title: 19-custompalette
 * Author: P Flynn
 * Date: 16-Sep-2025
 */

#include <stdio.h>
#include <neo/graphics.h>
#include <neo/console.h>

#define SC_W 320
#define SC_H 240

#define GRID_W (SC_W/16)
#define GRID_H (SC_H/16)

#define TRUE (0==0)
#define FALSE (0==1)

void wait_frames(uint16_t nf) {
  for(;nf>0;nf--) {
    uint32_t fc = neo_graphics_frame_count();
    while (fc == neo_graphics_frame_count()) ;
    }
  }

void palette_ramp(uint8_t red, uint8_t green, uint8_t blue) {
  for(uint16_t i=0;i<256;i++)
    neo_graphics_set_palette(i,red*i,green*i,blue*i);
  }

void draw_boxes(void) {
  for(uint8_t i=0; i<16; i++)
    for(uint8_t j=0;j<16;j++) { // y coordinate
	  uint8_t color = i*16+j;
	  neo_graphics_set_color(color);
	  neo_graphics_draw_rectangle(j*GRID_W,i*GRID_H,(j+1)*GRID_W -1, (i+1)*GRID_H -1);
	  }
  }

  
int main(void) {

	neo_graphics_set_solid_flag(TRUE);

    while (1==1) {
      palette_ramp(1,0,0); // red ramp
      draw_boxes();
      wait_frames(100);
      palette_ramp(0,1,0); // green ramp
      draw_boxes();
      wait_frames(100);
      palette_ramp(0,0,1); // blue ramp
      draw_boxes();
      wait_frames(100);
      }
}
