#include "../string.h"
#include "../terminal.h"

int runFile(char* filename);
void listRoot();

void _start() {
    print("OS Shell v1.0. Type \"help\" for help.\r\n");
    char input[0x100];
    while(1) {
        print("Shell> ");
        scan(input);
        if (strcmp(input, "help")) {
            print("This is a shell running independently from the OS kernel.\r\n");
            print("It is capable of more the the kernel's shell.\r\n");
            print("Type a command to run an executable file.\r\n");
            print("Or use one of the following built-in commands:\r\n");
            print("help: Prints this message.\r\n");
            print("exit: Returns to the OS kernel.\r\n");
            print("list: Lists the contents of the root directory.\r\n");
        }
        else if (strcmp(input, "exit")) {
            return;
        }
        else if (strcmp(input, "list")) {
            listRoot();
        }
        else {
            int status = runFile(input);
            if (status == 1) {
                print("\"");
                print(input);
                print("\" is not an external or built-in command. Try again.\r\n");
            }
        }
    }
}
