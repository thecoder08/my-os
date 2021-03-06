#include "terminal.h"
#include "mem.h"
#include "string.h"
#include "vga.h"
#include "fdc.h"
#include "parallel.h"

void kmain() {
      char* input = malloc(0x100);
      enableCursor(14, 15);
      clear(' ', 0x07);
      print("Welcome to My OS! Use the \"help\" command for help. Loading programs from disk isn't yet supported so only builtin commands will work.\r\n");
      while (1) {
            print("> ");
            scan(input);
            if (strcmp(input, "")) {}
            else if (strcmp(input, "help")) {
                  print("help: Print this message.\r\n");
                  print("clear: Clear the screen.\r\n");
                  print("cttyVGA: Change the TTY to VGA.\r\n");
                  print("cttySERIAL: Change the TTY to the serial port.\r\n");
                  print("halt: Stop the OS.\r\n");
                  print("floppy: Try reading from the floppy drive.\r\n");
                  print("graphics: Try changing the graphics mode and drawing to the screen.\r\n");
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
            else if (strcmp(input, "floppy")) {
                  char* buffer = malloc(512);
                  read_blocks(1, 0, 0, buffer);
                  print(buffer);
            }
            else if (strcmp(input, "graphics")) {
                  setVideoMode(1);
                  memset((char*) 0x000a0000, 4, 50);
            }
            else {
                  print("I'm not sure what \"");
                  print(input);
                  print("\" means.\r\n");
            }
      }
}