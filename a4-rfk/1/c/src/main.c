/* 
/ Title: a4-rfk
/
/ Description: robotfindskitten (classic) for Neo
/
/ Author:
/
/ Created:
/ Last Updated:
/
/ Modinfo:
*/

#include <stdio.h>
#include <neo6502.h>
#include <neo/console.h>
#include <neo/system.h>
#include <neo/sound.h>

#include "controller.h"
#include "math.h"
#include "nkistr.h"

// get the dpad part of the controller report (low nybble of low byte)
#define DPAD(x) ((x)&0x0f)
// masks for the various directions
#define DP_L 0x01
#define DP_R 0x02
#define DP_U 0x04
#define DP_D 0x08


typedef struct _rknki {
 uint8_t x,y,color;
 char ch;
 const char *desc;
 } rknki_t;

#define N_NKI 20
#define N_NKI_DESC 100

rknki_t nki[N_NKI];
rknki_t kitten;
rknki_t robot;

uint8_t g_sch, g_scw;
char g_status[80];

#define TRUE (1==1)
#define FALSE (1==0)

#define RAND(mx) (neo_math_random_integer(mx)) 
 
uint8_t randcolor() { // a random nonblack color in the default palette
  uint8_t c = 1 + RAND(15);
  while (c==8) c = 1 + RAND(15);
  return c;
  }

#define RANDCH() (33+RAND(127-33))

char randch(void) { // any printable char except robot's #
  char c = 33 + RAND(127-33);
  while (c == '#') 
    c = 33 + RAND(127-33);
  return c;
  }

void init(void) {
  uint8_t i,j;

  neo_console_screen_size(&g_scw,&g_sch);
  //printf("%d x %x\n",g_sch,g_scw);

  // place kitten <3
  kitten.y = 2+ RAND(g_sch-2);
  kitten.x = RAND(g_scw);
  kitten.color = randcolor(); 
  kitten.ch = randch();  
  //printf("k: (%d %d) color %d ch %c\n",kitten.x, kitten.y, kitten.color, kitten.ch);
  
  // place robot
  robot.y = 2+RAND(g_sch-2);
  robot.x = RAND(g_scw);
  robot.color = 7;
  robot.ch = '#';

  // make sure not overlap kitten
  while ((robot.y == kitten.y) && (robot.x == kitten.x)) {
    robot.y = 2+RAND(g_sch-2);
    robot.x = RAND(g_scw);
    }
  //printf("r: (%d %d) color %d ch %c\n",robot.x, robot.y, robot.color, robot.ch);

  // place NKIs
  for(i=0;i<N_NKI;i++) {
    uint8_t ready  = FALSE;
    // make sure not overlap kitten or robot or any of the other NKIs
    // also make sure description is different from other NKIs
    // also make sure character is different from other NKIs and kitten
    // initial values, hope they work
    uint8_t y = 2+RAND(g_sch-2), x = RAND(g_scw), di = RAND(N_NKI_DESC);
    char ch = randch();
    while (!ready) {
      ready=TRUE;
      if ((y==kitten.y) && (x==kitten.x)) { ready=FALSE; break; /* d'oh */ }
      if (ch == kitten.ch) { ready=FALSE; break; }
      if ((y==robot.y) && (x==robot.x)) { ready=FALSE; break; }
      if (i>0) {
        for(j=0;j<i;j++) {
          if ((y==nki[j].y) && (x==nki[j].x)) { ready=FALSE; break; }
          if (nki_descriptions[di] == nki[j].desc) { ready=FALSE; break; }
          if (ch == nki[j].ch) { ready=FALSE; break; }
          }
        }
      }
    nki[i].y=y; nki[i].x=x;
    nki[i].color = randcolor();
    nki[i].ch = ch;
    nki[i].desc = nki_descriptions[di];
    //printf("nki[%d]: (%d %d) color %d ch %c desc [%s]\n",i,nki[i].x, nki[i].y, nki[i].color, nki[i].ch,nki[i].desc);
    }
  }

void draw_rknki(rknki_t rk) {
  neo_console_set_cursor_pos(rk.x,rk.y);
  neo_console_set_text_color(rk.color,0);
  neo_console_write_char(rk.ch);
  }

void clear_status() { for(uint8_t i=0;i<sizeof(g_status);i++) g_status[i]=' ';}

void update_status() {
  neo_console_set_cursor_pos(0,0);
  puts(g_status);
  neo_console_set_cursor_pos(0,1);
  puts("_____________________________________________________");
  }

void render_board() {
  neo_console_clear_screen();
  update_status();
  draw_rknki(kitten);
  draw_rknki(robot);
  for(uint8_t i=0;i<N_NKI;i++) draw_rknki(nki[i]);
  }

void wait_ticks(uint16_t n) {
  while (n--) {
    uint32_t t = neo_system_timer();
    while (t == neo_system_timer()) ;
    }
  }
  
void rfk(void) { // robot found kitten!
  clear_status();
  sprintf(g_status,"robot found kitten! <3 <3 <3");
  update_status();
  wait_ticks(200);
  
  char tmpstat[g_scw];
  clear_status();
  sprintf(g_status,"R       <3       K           ");
  update_status();
  for(uint8_t i=1;i<8;i++) {
    g_status[i] = g_status[i-1];
    g_status[17-i] = g_status[18-i]; 
    g_status[i-1] = g_status[18-i] = ' ';
    update_status();
    wait_ticks(25);
    }
  }
   
  
void rfnki(int inki) {
  clear_status();
  sprintf(g_status,"%s",nki[inki].desc);
  update_status();
  }
  
// returns 0 if rfk, 1 if game should continue

uint8_t handle_controller() {
  uint8_t dpad = 0;
  while ((dpad = DPAD(neo_controller_read(neo_controller_count()))) == 0) /* wait */ ;
  int newx = robot.x, newy=robot.y;
  if (dpad & DP_L) newx--;
  if (dpad & DP_R) newx++;
  if (dpad & DP_U) newy--;
  if (dpad & DP_D) newy++;

  if ((newx >= 0) && (newx < g_scw) && (newy >= 2) && (newy < g_sch)) {
    if ((newx == kitten.x) && (newy == kitten.y)) { // <3
      rfk(); //  modal
      return FALSE;
      }
    for(uint8_t i=0;i<N_NKI;i++)
      if ((newx == nki[i].x) && (newy == nki[i].y)) {
        rfnki(i);
        return TRUE;
        }
    robot.x = newx;
    robot.y = newy;
    return TRUE;
    }
  else {
    return TRUE;
    }
  }
   

int main(void) {
  init();
  clear_status();
  sprintf(g_status,":)");
  render_board();
  while (handle_controller()) {
    render_board();
    wait_ticks(10);
    }
  return 0;
  }
