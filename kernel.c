#include "terminal.h"
#include "vga.h"
#include "mem.h"
#include "string.h"

void kmain() {
      char* input = malloc(100);
      clear(' ', 0x07);
      vgaPrint("Welcome to My OS! Use the serial terminal to interact with the system.\r\n");
      print("Welcome to My OS! Use the \"help\" command for help. Loading modules from disk isn't supported (yet) so only preloaded modules are usable.\r\n");
      while (1) {
            print("> ");
            scan(input);
            if (strcmp(input, "help")) {
                  print("My OS is based on the concept of modules. Modules are independent programs that run in ring-0. This means that they have full access to the hardware. Modules register themselves with the kernel to provide a command the user can run.\r\n");
                  print("Loaded modules:\r\n");
            }
            else {
                  print("I'm not sure what \"");
                  print(input);
                  print("\" means.\r\n");
            }
      }
}
