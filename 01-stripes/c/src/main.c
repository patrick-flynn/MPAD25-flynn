/* 
 * Title: 04-movingdot - C example
 * Author: P Flynn
 * Date: 19-Apr-2025
*/

#include <stdio.h>
#include <neo/graphics.h>

int main(void) {
	uint16_t i,j;
	uint8_t color;

	printf("Hi.");

	for(i=0;i<200;i++)
		for(j=0;j<200;j++) {
			neo_graphics_set_color(i);
			neo_graphics_draw_pixel(i,j);
		}


   return 0;

}
