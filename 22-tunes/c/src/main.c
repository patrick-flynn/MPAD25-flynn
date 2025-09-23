/* 
 * Title: 22-tunes
 * Author: P Flynn
 * Date: 22-Sep-2025
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
  for(i=0;i<sizeof(pitch);i++) {
    if (!strcmp(nn,pitch[i].name)) break;
    }
  if (i==sizeof(pitch)) {
    printf("Error: failed to find pitch for note named %s\n",nn);
    exit(-1);
    }
  return pitch[i].pitch;
  }

note_t song[2000];
uint16_t n_notes;

void wait_for_time(unsigned long t) {
  while (neo_system_timer() < t) /* wait */ ;
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
    
    n_notes=0;
    int ch=0;
    int oldst=0;

    while (TRUE) {
      char line[128],*p;
      char nn[4];
      uint16_t st,dur;
      uint16_t vel,tempo,pitch;
      uint16_t l = read_line(1,line);
      printf("line (%d) (n_notes %d): (length %d): %s\n",nl,n_notes,l,line);
      if (!l) break;
      if (nl++ == 0) continue; // skip header
      // format of line: 0,C5,0.0,249/500,64,180
      p = strtok(line,","); // index
      printf("index %s\n",p);
      p = strtok(NULL,","); //note name
      printf("name %s\n",p);
      pitch=findpitch(p); // corresponding pitch
      song[n_notes].pitch = pitch;
      p = strtok(NULL,","); // start time
      printf("start %s\n",p);
      sscanf(p,"%d",&st);
      song[n_notes].start_time = st/4;
      p = strtok(NULL,","); // duration
      printf("duration %s\n",p);
      sscanf(p,"%d",&dur);
      printf("pitch %d dur %d\n",pitch,dur);
      p = strtok(NULL,","); // velocity
      sscanf(p,"%d",&vel);
      song[n_notes].velocity = vel; 
      printf("velocity %d\n",vel);
      p = strtok(NULL,","); // tempo
      sscanf(p,"%d",&tempo);
      song[n_notes].tempo = tempo;
      printf("tempo %d\n",tempo);
      uint32_t tmpdur = (dur*100)/tempo;
      printf("tmpdur %lu\n",tmpdur);
      if (tmpdur > 65535) {
        printf("ERROR: crazy long duration-in-times-ticks %lu.\n",tmpdur);
        exit(-1);
        }
      song[n_notes].duration = tmpdur;
      song[n_notes].channel = ch++;
      if (st < oldst) {
        printf("ERROR: song notes not sorted by time.\n");
        exit(-1);
        }
      if (st != oldst) {
        oldst = st;
        ch = 0;
        }
      n_notes++;
      }
    printf("Read %d lines, %d notes.\n",nl,n_notes);

    // the song is in memory (ick). Play it.
    // the control algorithm is simple because the notes are sorted by start time, hopefully
    // (XXX should check this!)
    // we run the notes in order. If the start time is in the future, wait for it. if not, issue
    // the note.

    unsigned long t0 = neo_system_timer();
    uint16_t i;

    for (i=0;i<n_notes;i++) {
      note_t *p = &song[i];
      wait_for_time(t0+p->start_time);
      neo_sound_queue(p->channel,p->pitch,p->duration,0,0);
      }
   } 
