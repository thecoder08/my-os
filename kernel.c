#include "terminal.h"
#include "mem.h"
#include "string.h"
#include "vga.h"
#include "fdc.h"

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
                  print("cttySERIAL: Change the TTY to serial.\r\n");
                  print("halt: Stop the OS.\r\n");
                  print("write: Write a block of data to floppy drive.\r\n");
                  print("read: Read a block of data from floppy drive.\r\n");
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
            else if (strcmp(input, "read")) {
                  char* block = malloc(512);
                  read_blocks(1, 0, 0, block);
                  print(block);
            }
            else if (strcmp(input, "write")) {
                  char* block = malloc(512);
                  block[0] = 'y';
                  block[1] = 'o';
                  block[2] = 0;
                  write_blocks(1, 0, 0, block);
            }
            else {
                  print("I'm not sure what \"");
                  print(input);
                  print("\" means.\r\n");
            }
      }
}