/* 
/ Title:
/
/ Description:
/
/ Author:
/
/ Created:
/ Last Updated:
/
/ Modinfo:
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <neo6502.h>
#include <neo/console.h>
#include <neo/sound.h>
#include <neo/system.h>

int main(void) {
   
  uint16_t freq = API_NOTE_C4  ;
  neo_sound_queue(0,API_NOTE_C4,100,0,0);
  neo_sound_queue(0,API_NOTE_G4,100,0,0);
  //neo_sound_queue(2,API_NOTE_E4,10000,0,0);
  //neo_sound_queue(0,freq,10000,0,0);
  while(1==1);
  return 0;
}
