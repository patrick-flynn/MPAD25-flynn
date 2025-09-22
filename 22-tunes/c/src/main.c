/* 
 * Title: 22-tunes
 * Author: P Flynn
 * Date: 16-Sep-2025
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <neo/graphics.h>
#include <neo/console.h>
#include <neo/sound.h>
#include <neo/file.h>
#include <neo/system.h>

#include "note.h"

#define SC_W 320
#define SC_H 240

#define TRUE (0==0)
#define FALSE (0==1)

uint16_t read_line(uint8_t chan,char *buf) { // returns length
  uint16_t l=0;
  while (1==1) {
    char c;
    if (neo_file_eof(chan)) break;
    neo_file_read(chan,&c,1);
    buf[l++]=c;
    if (c == '\n') break;
    }
  if (l) // if file was not empty
    buf[l++]=0;
  return l;
  }

uint16_t findpitch(char *nn) {
  uint16_t i;
  for(i=0;i<sizeof(note);i++) {
    if (!strcmp(nn,note[i].name)) break;
    }
  if (i==sizeof(note)) {
    printf("Error: failed to find note %s\n",nn);
    exit(-1);
    }
  return note[i].pitch;
  }


int main(void) {
    uint8_t err;
    uint16_t nl=0;
    uint8_t nchan = neo_sound_channel_count();

    neo_file_open(1,"rpgchip01_title_screen.csv",NEO_FILE_READ);
    if ((err=neo_api_error())) {
      printf("neo_file_open(): error %d\n",err);
      exit(-1);
    }
    
    uint8_t c=0; // sound channel to use
    uint32_t oldst = 8675309; // jenny

    while (TRUE) {
      char line[128],*p;
      char nn[4];
      uint16_t st,dur;
      uint16_t vel,tempo,pitch;
      uint16_t l = read_line(1,line);
      printf("line (%d): (length %d): %s\n",nl,l,line);
      if (!l) break;
      if (nl++ == 0) continue; // skip header
      // format of line: 0,C5,0.0,249/500,64,180
      p = strtok(line,","); // index
      printf("index %s\n",p);
      p = strtok(NULL,","); //note name
      printf("name %s\n",p);
      pitch=findpitch(p); // corresponding pitch
      p = strtok(NULL,","); // start time
      printf("start %s\n",p);
      sscanf(p,"%d",&st);
      if (st == oldst)
        c++;
      else {
        oldst = st;
        c = 0;
        }
      p = strtok(NULL,","); // duration
      printf("duration %s\n",p);
      sscanf(p,"%d",&dur);
      printf("pitch %d dur %d\n",pitch,dur);
      printf("chan %d\n",c);
      if (c<nchan)
        neo_sound_queue(c,pitch,dur,0,0);
      else {
        printf("Error: ran out of channels.\n");
        //exit(-1);
        }
      }
    printf("Read %d lines.\n",nl);
    }
    
