/* 
/ Title: 11-controller
/
/ Description: explore gamepad, if any
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

int main(int argc,char *argv[]) {
  uint8_t nc,n,n2,i;
  uint32_t nn;


  while (1==1) {
    nc = neo_controller_count();
    printf("%d controllers\n",nc);
    if (nc > 0) {
      for(i=0;i<=nc;i++) {
        nn = neo_controller_read(i);
        printf("controller %d: 0x%lx\n",i,nn);
        }
      }

    // 100Hz timer
    nn = neo_system_timer();
    while ((neo_system_timer() - nn) < 100) /* wait */ ;
    }
  }
