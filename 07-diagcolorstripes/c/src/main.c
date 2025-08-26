/* 
/ Title: 07-colorwheel
/
/ Description: draw a color wheel
/
/ Author: Flynn	
/
/ Created: 26-Aug-2025	
/ Last Updated:
/
/ Modinfo:
*/

#include <stdio.h>
#include <kernel.h>
#include <neo6502.h>
#include <neo/graphics.h>
#include <neo/system.h>

#define X_SZ 320
#define Y_SZ 240

uint8_t br[] = {0,255,255,255,0,0,0,128,255,128,255,255,255,218,34,70};
uint8_t bg[] = {0,0,165,255,255,255,0,0,192,128,255,99,140,165,139,130};
uint8_t bb[] = {0,0,0,0,0,255,255,128,203,128,255,71,0,32,34,180};

void create_palette(void) {
 uint8_t i,ii;
 for(i=0;i<16;i++) {
   uint8_t r=br[i], g=bg[i], b=bb[i];
   for(ii=0;ii<16;ii++) {
     uint16_t red=(r*ii)>>4, green = (g*ii)>>4, blue = (b*ii)>>4;
     neo_graphics_set_palette(i<<4 | ii, red, green, blue);
     }
   }
  }

void draw_color_wheel(void) {
  uint16_t x,y;
  for(y=0;y<Y_SZ;y++) {
    for(x=0;x<X_SZ;x++) {
      neo_graphics_write_pixel(x,y,(x+y)%256);
      }
    }
  }

int main(int argc,char *argv[]) {
  create_palette();
  draw_color_wheel();
  }
