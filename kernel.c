#include "terminal.h"
#include "string.h"
#include "vga.h"

void kmain() {
      clear(' ', 0x07);
      vgaPrint("Welcome to My OS! Use the serial terminal to interact with the system.\r\n");
      print("Welcome to My OS! Use the \"help\" command for help. Loading modules from disk isn't supported (yet) so only preloaded modules are usable.\r\n");
      while (1) {
            print("> ");
            char* input = scan();
            if (strcmp(input, "help")) {
                  print("My OS is based on the concept of modules. Modules are independent programs that run in ring-0. This means that they have full access to the hardware. Modules register themselves with the kernel to provide a command the user can run.\r\n");
                  print("Loaded modules:\r\n");
                  print("hi\r\n");
            }
            else {
                  print(strcat("I'm not sure what \"", input));
                  print("\" means.\r\n");
            }
      }
}
