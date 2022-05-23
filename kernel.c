#include "terminal.h"
#include "mem.h"
#include "string.h"

void kmain() {
      char* input = malloc(100);
      clear(' ', 0x07);
      print("Welcome to My OS! Use the \"help\" command for help. Loading programs from disk isn't yet supported so only builtin commands are supported,\r\n");
      while (1) {
            vgaPrint("> ");
            keyboardScan(input);
            if (strcmp(input, "help")) {
                  vgaPrint("help: Print this message.\r\n");
                  vgaPrint("clear: Clear the screen.\r\n");
                  vgaPrint("cttyVGA: Change the TTY to VGA.");
                  vgaPrint("cttySERIAL: Change the TTY to serial.");
                  vgaPrint("halt: Stop the OS.\r\n");
            }
            else if (strcmp(input, "clear")) {
                  clear(' ', 0x07);
            }
            else if (strcmp(input, "cttyVGA")) {
                  ctty(1);
            }
            else if (strcmp(input, "cttySERIAL")) {
                  ctty(0);
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
