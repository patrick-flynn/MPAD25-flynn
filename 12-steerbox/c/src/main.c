/* 
/ Title: 12-steerbox
/
/ Description: steer a box around the screen with the dpad
/              change the type of shape with the YXBA buttons
/              works with keyboard (default) controller as well (WASD + OPKL or
/              arrow keys + ZXCV).  if a real controller is plugged in, the keyboard
/              won't be used.
/ Note: 
/
/ Author: Flynn	
/
/ Created: 27-Aug-2025	
/ Last Updated:
/
/ Modinfo:
*/

#include <stdio.h>
#include <stdlib.h>
#include <kernel.h>
#include <neo6502.h>
#include <neo/graphics.h>
#include <neo/system.h>
#include <neo/console.h>
#include <neo/file.h>
#include <neo/controller.h>

// screen size
#define SC_W 320
#define SC_H 240

// get the dpad part of the controller report (low nybble of low byte)
#define DPAD(x) ((x)&0x0f)
// masks for the various directions
#define DP_L 0x01
#define DP_R 0x02
#define DP_U 0x04
#define DP_D 0x08

// get the YXBA part of the controller report (high nybble of low byte)
#define YXBA(x) (((x)>>4)&0x0f)
// masks for Y, X, B, A parts
// note: if a DS4 controller is used, Y is square and X is triangle
// and B is cross and A is circle
#define C_Y 0x08
#define SQ (C_Y)
#define C_X 0x04
#define TR (C_X)
#define C_B 0x02
#define CR (C_B)
#define C_A 0x01
#define CI (C_A)

// handymacros for clipping coordinates
#define MAX(x,y) ((y)>(x) ? (y) : (x))
#define MIN(x,y) ((y)<(x) ? (y) : (x))

// draw a shape of a given type/size at a given position; in black if erase==1, in
// a hardcoded shape-specific color if erase==0

void draw(uint8_t type, uint16_t x, uint16_t y, uint8_t sz, uint8_t erase) {
	//printf("draw bt 0x%x x %d y %d sx %d erase %d\n",type,x,y,sz,erase);
	if (erase) {
		neo_graphics_set_color(COLOR_BLACK);
	} else {
		switch(type) {
			case SQ: neo_graphics_set_color(0x3); break;
			case TR: neo_graphics_set_color(0x2); break;
			case CR: neo_graphics_set_color(0xF); break;
			case CI: neo_graphics_set_color(0x1); break;
		}
	}
	switch(type) {
		case SQ:
			neo_graphics_draw_rectangle(x-sz/2, y-sz/2, x+sz/2, y+sz/2);
			break;
		case TR:
			neo_graphics_draw_line(x,y-sz/2,x-sz/2,y+sz/2);
			neo_graphics_draw_line(x,y-sz/2,x+sz/2,y+sz/2);
			neo_graphics_draw_line(x-sz/2,y+sz/2,x+sz/2,y+sz/2);
			break;
		case CR: 	
			neo_graphics_draw_line(x-sz/2,y-sz/2,x+sz/2,y+sz/2);
			neo_graphics_draw_line(x+sz/2,y-sz/2,x-sz/2,y+sz/2);
			break;
		case CI:
			neo_graphics_draw_ellipse(x-sz/2, y-sz/2, x+sz/2, y+sz/2);
			break;
	}
}


int main(int argc,char *argv[]) {
  uint16_t bx=SC_W/2,by=SC_H/2; // center of box
  uint8_t bs=16; // size of box
  uint8_t bt = SQ; // type of box

  draw(bt,bx,by,bs,0); // draw a 16x16 square at the middle of the screen

  while (1==1) {
	  uint32_t status = neo_controller_read(neo_controller_count());
	  // four cases: (1) nothing pressed, (1) dpad pressed, (2) shape pressed, (3) both pressed
	  if (status) {// rule out case 1
		// we must erase the old thing; it either changed position or type or both
	  	draw(bt,bx,by,bs,1); // erase
		// if it changed type, update the type
		if (YXBA(status)) bt = YXBA(status);
		// if multiple type buttons were down, select highest one
		bt = (bt&SQ) ? SQ : ((bt&TR) ? TR : ((bt&CR) ? CR : ((bt&CI) ? CI : 0)));
	        // figure out position	
	        uint8_t dp = DPAD(status);
		if (dp != 0) { 
	       	    if (dp & DP_R) bx = MIN(SC_W-bs/2, bx+1); // right means "increment x"
	            if (dp & DP_L) bx = MAX(bs/2,bx-1); // decrement x
	            if (dp & DP_U) by = MAX(bs/2,by-1); // dcrement y
	            if (dp & DP_D) by = MIN(SC_H-bs/2,by+1); // increment y
		}
	        draw(bt,bx,by,bs,0); // draw in updated position
	    }
	  // after we draw, wait for vertical sync. This will slow down the action.
	  // (try commenting it out, yikes)
	  uint32_t fc = neo_graphics_frame_count();
	  while (fc == neo_graphics_frame_count()) /* wait */ ;
  }
}
