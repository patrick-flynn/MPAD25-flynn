/* 
/ Title: 17-busybox
/
/ Description: explore all the things!
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

// local-API-supoprt files (copy them here)
// if these API updated get incorporated into the
// official llvm-mos-sdk repo, you would replace them with
// #include <neo/mouse.h>
// #include <neo/controller.h>
// #include <neo/math.h>
// Note: as of this writing, llvm-mos-sdk actually has an updated controller.h in main,
// but not in the releases.

#include "mouse.h"
#include "controller.h"
#include "math.h"

int main(void) {

   printf("Hello, World!\n");

   return 0;

}
