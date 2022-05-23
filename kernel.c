#include "terminal.h"
#include "mem.h"
#include "string.h"
#include "vga.h"

void kmain() {
      char* input = malloc(100);
      clear(' ', 0x07);
      print("Welcome to My OS! Use the \"help\" command for help. Loading programs from disk isn't yet supported so only builtin commands will work.\r\n");
      while (1) {
            print("> ");
            scan(input);
            if (strcmp(input, "help")) {
                  print("help: Print this message.\r\n");
                  print("clear: Clear the screen.\r\n");
                  print("cttyVGA: Change the TTY to VGA.\r\n");
                  print("cttySERIAL: Change the TTY to serial.\r\n");
                  print("halt: Stop the OS.\r\n");
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
                  print("I'm not sure what \"");
                  print(input);
                  print("\" means.\r\n");
            }
      }
}