/* 
 * Title: 18-palette
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

int main(void) {
	uint16_t i,j;
	uint8_t color=0;

    neo_graphics_set_solid_flag(TRUE);

    for(i=0;i<16;i++) { // x coordinate
      for(j=0;j<16;j++) { // y coordinate
        color = i*16+j;
        neo_graphics_set_color(color);
        // x1, y1, x2, y2
        neo_graphics_draw_rectangle(j*GRID_W,i*GRID_H,(j+1)*GRID_W -1, (i+1)*GRID_H -1);
        }
      }

    // draw a grid line in white
    neo_graphics_set_color(7);

    for(i=0;i<16;i++) {
      neo_graphics_draw_line(0,i*GRID_H,319,i*GRID_H);
      neo_graphics_draw_line(i*GRID_W,0,i*GRID_W,239);
      }
    neo_graphics_draw_line(0,239,319,239);
    neo_graphics_draw_line(319,0,319,239);

    while (1==1);
    }
