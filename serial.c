#include "io.h"
#include "serial.h"

void serialPrint(char* string) {
    for (int i = 0; string[i] != 0; i++) {
        out(0x03f8, string[i]);
    }
}

void serialScan(char* buffer) {
    int index = 0;
    while(1) {
        if (in(0x03fd) % 2) {
            unsigned char inByte = in(0x03f8);
            if (inByte == '\r') {
                buffer[index] = 0;
                serialPrint("\r\n");
                return;
            }
            else if (inByte == '\b') {
                if (index > 0) {
                    index--;
                    serialPrint("\b \b");
                }
            }
            else {
                buffer[index] = inByte;
                out(0x03f8, buffer[index]);
                index++;
            }
        }
    }
}
