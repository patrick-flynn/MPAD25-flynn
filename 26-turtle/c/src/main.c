/* 
/ Title: 26-turtle
/
/ Description: turtling around the screen
/
/ Author: PJF
/
/ Created: 25-sep-2025
/ Last Updated:
/
/ Modinfo:
*/

#include <stdio.h>
#include <neo6502.h>
#include <neo/turtle.h>
#include <neo/console.h>
#include <neo/file.h>
#include "math.h"

#define TRUE (1==1)

#define WAITKEY {while(!neo_console_read_char()) ;}

int main(void) {

   neo_turtle_init(0);
   neo_turtle_home();
   neo_turtle_show();
   WAITKEY;

   for(uint8_t i=0;i<36;i++) {
     for(uint8_t j=0;j<5;j++) {
       neo_turtle_move(100,2,TRUE);
       neo_turtle_turn(144);
       WAITKEY;
       }
     neo_turtle_turn(10);
     }

   while (1==1);
     


   return 0;

}
