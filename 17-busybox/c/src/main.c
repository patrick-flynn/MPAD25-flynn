/* 
/ Title: 17-busybox
/
/ Description: mouse & keyboard & controller, making things happen on the
/ screen. Additional things also happen without user action.
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
#include <neo/console.h>
#include <neo/file.h>
#include <neo/sprites.h>
#include <neo/sound.h>

// local-API-support files (copy them here)
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


#define SC_W 320
#define SC_H 240

#define TRUE (1==1)
#define FALSE (1==0)

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

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

#define GFX_FILE_NAME "sprite.gfx"

// sprite numbers start at 1 (turtle owns 0)
#define SPRITE_NUMBER 1
// "position" of sprite in gfx (16x16 sprites start at 0x80)
#define SPRITE_ID 0x80
// sprite size in pixels
#define SPRITE_SZ 16

// various globals
uint16_t g_sprx = 0,g_spry = 0;
int8_t g_vsprx=0,g_vspry=0;
uint8_t g_mouse=0, g_ncontrollers=0;
uint16_t g_mousex=SC_W/2, g_mousey=SC_H/2;

// scoring, pew pew!
uint32_t g_score=0;
uint8_t g_csw,g_csh; // console size w, h

void update_status(char *s) {
  neo_console_set_cursor_pos(g_csw/2,g_csh-1);
  if (s) {
    neo_console_set_text_color(6,0); // cyan on black?
    printf("status: %s",s);
    }
  else {
    neo_console_set_text_color(0,0); // black-on-black ?
    for(int i=0;i<g_csw/2;i++) printf(" ");
    }
  }

void update_score() {
  neo_console_set_text_color(5,7); // magenta on white
  neo_console_set_cursor_pos(0,g_csh-1);
  printf("SCORE: %lu",g_score);
  }

void initialize(void) {
  uint8_t i;

  // read sprite
  neo_file_load(GFX_FILE_NAME,NEO_FILE_DESTINATION_GRAPHICS);

  // set sprite position
  g_sprx = neo_math_random_integer(SC_W-SPRITE_SZ) + SPRITE_SZ/2; 
  g_spry = neo_math_random_integer(SC_W-SPRITE_SZ) + SPRITE_SZ/2; 

  // and velocity
  // avoid zero velocity components, they are BORING
  uint32_t r = neo_math_random_integer(256);
  g_vsprx = 1+(r%0x7)%4; // 1 to 4
  g_vspry = 1+((r>>3)%0x7)%4; // 1 to 4
  if (r&0x40) g_vsprx *= -1; // flip sign randomly
  if (r&0x80) g_vspry *= -1; // flip sign randomly
  

  g_mouse = neo_mouse_test_mouse_present();
  neo_mouse_set_mouse_display_cursor(TRUE);
  neo_mouse_move_display_cursor(g_mousex,g_mousey);

  g_ncontrollers = neo_controller_count();

  neo_graphics_set_solid_flag(TRUE);

  neo_console_screen_size(&g_csw,&g_csh);
  neo_console_set_cursor_pos(0,0);

  update_status("READY");
  return;
  }


int main(void) {
  uint8_t wasPressed = FALSE;

  initialize();

  // THE GAME LOOP
  while (1==1) {

    update_status((char *)0);
    update_score();

    // DO THINGS AND DRAW THINGS

    // every once in a while, draw a distracting randomly positioned,
    // sized, and colored rectangle
    if ((neo_graphics_frame_count() % 120) == 0) { // every 120 frames 
      // something graphically interesting
      uint16_t x1,y1,x2,y2,c,t;
      x1 = neo_math_random_integer(SC_W);
      x2 = neo_math_random_integer(SC_W);
      if (x1>x2) { t = x1; x1=x2; x2=t; };
      y1 = neo_math_random_integer(SC_H);
      y2 = neo_math_random_integer(SC_H);
      if (y1>y2) { t = y1; y1=y2; y2=t; };
      c = neo_math_random_integer(15) + 1;
      neo_graphics_set_color(c);
      neo_graphics_draw_rectangle(x1,y1,x2,y2);
      update_status("RECT");
      neo_sound_play_effect(0,23);
      }

    // somewhat more frequently, move the sprite, bouncingly 
    if ((neo_graphics_frame_count() % 2) == 0) { 
      // draw sprite, wherever it currently is
      neo_sprite_set(SPRITE_NUMBER, g_sprx, g_spry, (SPRITE_ID & 0x7F),0,0);
      // update position
      g_sprx += g_vsprx;
      g_spry += g_vspry;
      // x bounce?
      if (g_sprx > SC_W - SPRITE_SZ/2) {
        update_status("BOUNCE");
        neo_sound_play_effect(0,6);
        g_sprx = SC_W - SPRITE_SZ/2; 
        g_vsprx *= -1;
        } else if (g_sprx < SPRITE_SZ/2) {
        update_status("BOUNCE");
        neo_sound_play_effect(0,6);
        g_sprx = SPRITE_SZ/2; 
        g_vsprx *= -1;
        }

      // y bounce?
      if (g_spry > SC_H - SPRITE_SZ/2) {
        update_status("BOUNCE");
        neo_sound_play_effect(0,6);
        g_spry = SC_H - SPRITE_SZ/2; 
        g_vspry *= -1;
        } else if (g_spry < SPRITE_SZ/2) {
        update_status("BOUNCE");
        neo_sound_play_effect(0,6);
        g_spry = SPRITE_SZ/2; 
        g_vspry *= -1;
        }
      }

    // we have to heep track of the mouse
    // update mouse position globals using mouse movements
    uint16_t mxPos,myPos;
    uint8_t mBS,mSWS;
    neo_mouse_get_mouse_state(&mxPos,&myPos,&mBS,&mSWS);

    if (mxPos != g_mousex) {
      g_mousex = mxPos;
      update_status("MOUSEX");
      }
    if (myPos != g_mousey) {
      g_mousey = myPos;
      update_status("MOUSEY");
      }

    // controller dpad presses also update mouse coords 
    // read the status of the highest-numbered attached/recognized controller
    uint32_t cs = neo_controller_read(g_ncontrollers);
    uint8_t x = cs&0xFF;
    uint8_t dpad = DPAD(x), yxba = YXBA(x);
    uint16_t newx=g_mousex,newy=g_mousey;
    // if dpad buttons pushed, update coords but clamp them to the visible area
    if (dpad & DP_L) newx = MAX(0,newx-1); 
    if (dpad & DP_R) newx = MIN(0,newx+1); 
    if (dpad & DP_U) newy = MAX(0,newy-1); 
    if (dpad & DP_D) newy = MIN(0,newy+1); 
    if (newx != g_mousex) {
      update_status("DPADX");
      g_mousex = newx;
      }
    if (newy != g_mousey) {
      update_status("DPADY");
      g_mousey = newy;
      }
    // move the pointer for reals
    neo_mouse_move_display_cursor(g_mousex,g_mousey);

    // now that we have an updated mouse, see if a button is down and
    // the mouse position is inside the sprite (pew pew!) and score a point
    // if so
    // In order to get one point per click and not a bunch of points while the
    // button is still down, interlock on wasPressed
    if (mBS || yxba) { // button down on mouse or controller
      if (((g_mousex >= g_sprx - SPRITE_SZ/2) &&
           (g_mousex <= g_sprx + SPRITE_SZ/2) ) &&
          ((g_mousey >= g_spry - SPRITE_SZ/2) &&
           (g_mousey <= g_spry + SPRITE_SZ/2) && (!wasPressed) )) {
        update_status("PEW PEW");
        neo_sound_play_effect(0,8);
        g_score += 1;
        wasPressed = TRUE; // we got our point. No more until we release button
        }
      }
    else {
      wasPressed = FALSE;
      }

    uint32_t fc = neo_graphics_frame_count();
    while (fc == neo_graphics_frame_count()) /* wait */ ;
    }
}
