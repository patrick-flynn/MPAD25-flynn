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


int main(void) {
   //neo_console_clear_screen();
   asm volatile("LDA #$0C"); // neo_console_clear_screen
   asm volatile("STA #$FF01");
   asm volatile("LDA #$02"); // console
   asm volatile("STA #$FF00"); // off you go
   asm volatile("JSR $FFF4"); // KWaitMessage

   //neo_console_set_cursor_pos(0,0);
   asm volatile("STZ $FF04"); // (0,
   asm volatile("STZ $FF05"); //    0)
   asm volatile("LDA #$07");  // neo_console_set_cursor_pos
   asm volatile("STA $FF01"); //    0)
   asm volatile("LDA #$02"); // console
   asm volatile("STA #$FF00"); // off you go
   asm volatile("JSR $FFF4"); // KWaitMessage

   asm volatile("
   //for (char *c="Hello, world!"; *c; c++) {
     //neo_console_write_char(*c);
     //}
   //return 0;

}
