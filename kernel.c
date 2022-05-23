#include "vga.h"
#include "keyboard.h"
#include "mem.h"
#include "string.h"

void kmain() {
      char* input = malloc(100);
      clear(' ', 0x07);
      vgaPrint("Welcome to My OS! Use the \"help\" command for help. Loading modules from disk isn't supported (yet) so only preloaded modules are usable.\r\n");
      while (1) {
            vgaPrint("> ");
            keyboardScan(input);
            if (strcmp(input, "help")) {
                  vgaPrint("clear: clear the screen");
                  vgaPrint("My OS is based on the concept of modules. Modules are independent programs that run in ring-0. This means that they have full access to the hardware. Modules register themselves with the kernel to provide a command the user can run.\r\n");
                  vgaPrint("Loaded modules:\r\n");
            }
            if (strcmp(input, "clear")) {
                  clear(' ', 0x07);
            }
            else {
                  vgaPrint("I'm not sure what \"");
                  vgaPrint(input);
                  vgaPrint("\" means.\r\n");
            }
      }
}
