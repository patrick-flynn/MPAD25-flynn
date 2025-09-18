/* 
 * Title: 20-imagebkgnd
 * Author: P Flynn
 * Date: 16-Sep-2025
 *
 * this file reads a palette from imgpalette.bin (240 rgb entries)
 * and 320x240 pixels (palette indices) from imgbytes.bin
 * and creates a background image
 */

#include <stdio.h>
#include <neo/graphics.h>
#include <neo/console.h>
#include <neo/file.h>

#define SC_W 320
#define SC_H 240

#define TRUE (0==0)
#define FALSE (0==1)

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
          //for(j=0;j<SC_W;j++) {
             //uint8_t p;
          neo_file_read(1,&p,SC_W);
          for(j=0;j<SC_W;j++) neo_graphics_write_pixel(j,i,p[j]);
             //neo_file_read(1,&p,1); // HAHAHAHAHAHA
             //neo_graphics_write_pixel(j,i,p);
             //};
          }
        neo_file_close(1);
        }
