/* 
/ Title: 24-music
/
/ Description: play tunes on the Neo, using a primitive
/ text description derived from musicxml by an external python script.
/
/ Author: P Flynn
/
/ Created: 11-7-2025
/ Last Updated:
/
/ Modinfo:
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <neo6502.h>
#include <neo/console.h>
#include "sound.h"
#include <neo/system.h>
#include <neo/file.h>

//#define FN "SailOnBoys.csv"
//#define FN "the-deep-river-samba-traditionalish.csv"
#define FN "super-mario-bros-main-theme.csv"

typedef struct {
  uint16_t offset;
  uint16_t pitch;
  uint16_t end;
  } note_t;

// playback "speed factor" - 2 is normalish; higher is faster, lower is slower
#define SPEED 2


#define NMAX 1024
note_t note[NMAX];
uint16_t nn;

typedef struct {
  char nstr[5];
  uint16_t apinote;
  } notemap_t;

notemap_t notemap[] = { 
{"C0",0x0010}, {"D-0",0x0011}, {"D0",0x0012},
{"E-0",0x0013}, {"E0",0x0015}, {"F0",0x0016},
{"G-0",0x0017}, {"G0",0x0018}, {"A-0",0x001A},
{"A0",0x001C}, {"B-0",0x001D},
{"B0",0x001F}, {"C1",0x0021}, {"D-1",0x0023},
{"D1",0x0025},  {"E-1",0x0027}, {"E1",0x0029},
{"F1",0x002C},  {"G-1",0x002E}, {"G1",0x0031},
{"A-1",0x0034}, {"A1",0x0037},
{"B-1",0x003A}, {"B1",0x003E}, {"C2",0x0041},
{"D-2",0x0045}, {"D2",0x0049}, {"E-2",0x004E},
{"E2",0x0052}, {"F2",0x0057}, {"G-2",0x005C},
{"G2",0x0062}, {"A-2",0x0068}, {"A2",0x006E},
{"B-2",0x0075}, {"B2",0x007B}, {"C-3",0x007B}, {"C3",0x0083},
{"D-3",0x008B}, {"D3",0x0093}, 
{"E-3",0x009C}, {"E3",0x00A5}, {"F3",0x00AF},
{"F#3",0x00B9}, {"G-3",0x00B9}, {"G3",0x00C4}, {"A-3",0x00D0},
{"A3",0x00DC}, {"B-3",0x00E9}, {"B3",0x00F7},
{"C4",0x0106}, {"D-4",0x0115}, {"D4",0x0126},
{"E-4",0x0137}, {"E4",0x014A}, {"F4",0x015D},
{"G-4",0x0172}, {"G4",0x0188}, {"G#4",0x019F},{"A-4",0x019F},
{"A4",0x01B8},  {"B-4",0x01D2},
{"B4",0x01EE}, {"C5",0x020B}, {"C#5",0x022A},{"D-5",0x022A},
{"D5",0x024B}, {"D#5",0x026E}, {"E-5",0x026E}, {"E5",0x0293},
{"F5",0x02BA}, {"F#5",0x02E4}, {"G-5",0x02E4}, {"G5",0x0310},
{"A-5",0x033F}, {"A5",0x0370}, 
{"B-5",0x03A4}, {"B5",0x03DC}, {"C6",0x0417}, 
{"D-6",0x0455}, {"D6",0x0497}, {"E-6",0x04DD},
{"E6",0x0527}, {"F6",0x0575},  {"G-6",0x05C8},
{"G6",0x0620}, {"A-6",0x067D}, {"A6",0x06E0},
{"B-6",0x0749}, {"B6",0x07B8}, {"C7",0x082D},
{"D-7",0x08A9}, {"D7",0x092D},
{"E-7",0x09B9}, {"E7",0x0A4D}, {"F7",0x0AEA},
{"G-7",0x0B90}, {"G7",0x0C40}, {"A-7",0x0CFA},
{"A7",0x0DC0},  {"B-7",0x0E91}, {"B7",0x0F6F},
{"C8",0x105A},  {"D-8",0x1153}, {"D8",0x125B},
{"E-8",0x1372}, {"E8",0x149A}, {"F8",0x15D4},
{"G-8",0x1720}, {"G8",0x1880}, {"A-8",0x19F5},
{"A8",0x1B80}, {"B-8",0x1D23},
{"B8",0x1EDE}, {"C9",0x20B4},{"D-9",0x22A6},
{"D9",0x24B5},  {"E-9",0x26E4}, {"E9",0x2934},
{"F9",0x2BA7}, {"G-9",0x2E40}, {"G9",0x3100},
{"A-9",0x33EA}, {"A9",0x3700},
{"B-9",0x3A45}, {"B9",0x3DBC}, {"C10",0x4168},
{"D-10",0x454C}, {"D10",0x496B}, {"E-10",0x4DC8},
{"\0\0\0\0\0",0}};

uint16_t getnote(char *str) {
  notemap_t *p;
  for(p=notemap;
      (p->nstr[0] != '\0') && strcmp(str,p->nstr);
      p++) ;
  if (p->nstr[0] == '\0') {
    printf("ERROR: no API note value for note %s\n",str);
    exit(-1);
    }
   return p->apinote;
}

int main(void) {
  nn=0;
  //printf("%d sound channels.\n",neo_sound_channel_count());

  uint8_t chan=1;
  neo_file_open(chan,FN,NEO_FILE_READ);

  //printf("open.\n");
  while(1==1) {
    char line[80],*ptr=line, *limit = ptr+80;;
    char *ostr, *nstr, *estr;
    uint16_t offset, pitch, end;
    //printf("line %p\n",line);
    for( ; ; ptr++) {
      neo_file_read(chan,ptr,1);
      //printf("ptr %p read '%c' [0x%x]\n",*ptr,*ptr);
      if (neo_file_eof(chan)) break;
      if (*ptr == 0x0a) break;
      if (ptr == limit) break;
      }
    if (ptr == limit) {
      printf("ERROR: buffer overflow\n");
      exit(-1);
      };
    if (neo_file_eof(chan)) {
      neo_file_close(chan);
      break;
      }
    // clobber newline
    *ptr = '\0';
    //printf("read [%s]\n",line);
    //exit(-1);
    // we have a line in str. chop it into pieces
    ostr = strtok(line,",");
    // convert to int
    //printf("ostr %s\n",ostr);
    offset = atoi(ostr)/SPEED;
    //printf("offset %d\n",offset);
    nstr = strtok(NULL,",");
    //printf("nstr %s\n",nstr);
    pitch = getnote(nstr); // API pitch
    //printf("pitch 0x%x\n",pitch);
    estr = strtok(NULL,",");
    //printf("dstr %s\n",dstr);
    end = atoi(estr)/SPEED; // duration in ticks at specified qpm 
    note[nn].offset = offset;
    note[nn].pitch = pitch;
    note[nn].end = end;
    //printf("read note %d: [%d %p %d]\n",nn,note[nn].offset,note[nn].pitch,note[nn].end);
    nn++;
    if (nn== NMAX) {
      printf("Note buffer too small.\n");
      exit(-1);
      }
    }
  //printf("play (%d notes).\n",nn);

  // wait
  printf("Press any key:");
  while (!neo_console_read_char()) /* wait */ ;

  // now play the tune
  uint32_t t0 = neo_system_timer() + 10; // 0.1 second delay
  uint32_t oldt0 = t0;
  uint16_t index = 0;
  while (1==1) {
    uint8_t ch=0;
    uint32_t off = note[index].offset;
    printf("offset %d: ",off);
    // wait for timer to reach ofset for current note
    //while (neo_system_timer() < t0 + off) /* wait */;
    // alt: wait for 5 seconds to elapse
    //while (neo_system_timer() < oldt0 + 500) /* wait */;
    //oldt0 = neo_system_timer(); 
    //printf("CHUNK.\n");
    // play all notes with this offset
    while (note[index].offset == off) {
      // play up to channel_count sounds
      if (ch < neo_sound_channel_count()) {
        uint32_t tdur = note[index].end - off;
        uint16_t dur=tdur;
        printf("[%d:%p %d]",index,note[index].pitch,dur);
        neo_sound_queue(ch,note[index].pitch,dur,0,0);
        }
      ch++;
      index++;
      if (index >= nn) break;
      }
    printf("\n");
    if (index >= nn) break;
    }
  while (1==1) ;
}
