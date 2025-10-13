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

#include <neo/console.h>

int main(void) {
   neo_console_clear_screen();
   neo_console_set_cursor_pos(0,0);
   for (char *c="Hello, world!"; *c; c++) {
     neo_console_write_char(*c);
     }
   return 0;

}
