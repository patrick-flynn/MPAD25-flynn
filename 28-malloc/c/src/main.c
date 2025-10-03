/* 
/ Title: 28-malloc
/
/ Description: Test malloc a lot
/
/ Author: PJF
/
/ Created: 03-Oct-2025
/ Last Updated:
/
/ Modinfo:
*/

#include <stdio.h>
#include <stdlib.h>
#include <neo6502.h>
#include <neo/system.h>

int main(void) {
   char *ptr;
   uint32_t sz;
   uint32_t fail;

   printf("1: mallocs in powers of two, ascending.\n");
   for (sz=1,fail=0;sz<=32768;sz <<= 1) { 
     ptr = malloc(sz);
     //printf("%5d: %p\n",sz,ptr);
     if (ptr == NULL) fail++;
     else {
       printf("%5lu: %p\n",sz,ptr);
       free(ptr);
       }
     }


   printf("malloc() broke %lu times out of 15 \n",fail);

   uint32_t t = neo_system_timer();
   while (neo_system_timer() <= t+100) ; 

   for(sz=1,fail=0;sz<=65535;sz++) {
     ptr = malloc(sz);
     //printf("%5d: %p\n",sz,ptr);
     if (ptr == NULL) fail++;
     else {
       printf("%5lu: %p\n",sz,ptr);
       free(ptr);
       }
     }

   printf("malloc() broke %lu times out of 65535 \n",fail);

   return 0;

}
