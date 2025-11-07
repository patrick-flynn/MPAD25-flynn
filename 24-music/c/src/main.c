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
#include "sound.h"
#include <neo/system.h>
#include <neo/file.h>

#define FN "SailOnBoys.csv"

typedef struct {
  uint16_t offset;
  uint16_t pitch;
  uint16_t dur;
  } note_t;

#define NMAX 1024
note_t note[NMAX];
uint16_t nn=0;

// ticks for a note of a specified duration ("16th", "eighth"m etc.)
// at a specified tempo in quarter-notes per minute
uint16_t getdur(char *str,uint8_t qpm) {
  uint16_t dur;
  if (!strcmp(str,"16th")) dur = 6000/(qpm*16);
  else if (!strcmp(str,"eighth")) dur = 6000/(qpm*8);
  else if (!strcmp(str,"quarter")) dur = 6000/(qpm*4);
  else if (!strcmp(str,"half")) dur = 6000/(qpm*2);
  else if (!strcmp(str,"whole")) dur = 6000/(qpm*1);
  else
    printf("ERROR: unknown duration %s\n",str);
  return dur;
}

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
{"B-2",0x0075}, {"B2",0x007B}, {"C3",0x0083},
{"D-3",0x008B}, {"D3",0x0093}, 
{"E-3",0x009C}, {"E3",0x00A5}, {"F3",0x00AF},
{"G-3",0x00B9}, {"G3",0x00C4}, {"A-3",0x00D0},
{"A3",0x00DC}, {"B-3",0x00E9}, {"B3",0x00F7},
{"C4",0x0106}, {"D-4",0x0115}, {"D4",0x0126},
{"E-4",0x0137}, {"E4",0x014A}, {"F4",0x015D},
{"G-4",0x0172}, {"G4",0x0188}, {"A-4",0x019F},
{"A4",0x01B8},  {"B-4",0x01D2},
{"B4",0x01EE}, {"C5",0x020B}, {"D-5",0x022A},
{"D5",0x024B}, {"E-5",0x026E}, {"E5",0x0293},
{"F5",0x02BA},  {"G-5",0x02E4}, {"G5",0x0310},
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
  
  printf("%d sound channels.\n",neo_sound_channel_count());

  uint8_t chan=1;
  neo_file_open(chan,FN,NEO_FILE_READ);

  printf("open.\n");
  while(1==1) {
    char line[80],*ptr, *limit = ptr+80;;
    char *ostr, *nstr, *dstr;
    uint16_t offset, pitch, dur;
    for(ptr=line; ; ptr++) {
      neo_file_read(chan,ptr,1);
      printf("read %c\n",*ptr);
      if (neo_file_eof(chan)) {printf("eof.\n"); break;}
      if (*ptr == '\n') {printf("newline.\n"); break;}
      if (*ptr == limit) {printf("limit.\n"); break;}
      printf("not broke.\n");
      }
    printf("broke\n");
    if (ptr == limit) {
      printf("ERROR: buffer overflow\n");
      exit(-1);
      };
    printf("bla\n");
    printf("read [%s]\n",line);
    printf("bla2\n");
    exit(-1);
    // we have a line in str. chop it into pieces
    ostr = strtok(line,",");
    // convert to int
    offset = atoi(ostr);
    pitch = getnote(nstr); // API pitch
    dur = getdur(dstr,120); // duration in ticks at specified qpm 
    note[nn].offset = offset;
    note[nn].pitch = pitch;
    note[nn++].dur = dur;
    if (nn== NMAX) {
      printf("Note buffer too small.\n");
      exit(-1);
      }
    }
  exit(0);
}
