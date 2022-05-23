#include "vga.h"
#include "keyboard.h"
#include "mem.h"
#include "string.h"

void kmain() {
      char* input = malloc(100);
      clear(' ', 0x07);
      vgaPrint("Welcome to My OS! Use the \"help\" command for help. Loading programs from disk isn't yet supported so only builtin commands are supported,\r\n");
      while (1) {
            vgaPrint("> ");
            keyboardScan(input);
            if (strcmp(input, "help")) {
                  vgaPrint("help: print this message.\r\n");
                  vgaPrint("clear: clear the screen.\r\n");
                  vgaPrint("halt: stop the OS.\r\n");
            }
            else if (strcmp(input, "clear")) {
                  clear(' ', 0x07);
            }
            else if (strcmp(input, "halt")) {
                  while(1);
            }
            else {
                  vgaPrint("I'm not sure what \"");
                  vgaPrint(input);
                  vgaPrint("\" means.\r\n");
            }
      }
}
