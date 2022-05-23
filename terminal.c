#include "io.h"

void scan(char* buffer) {
    int index = 0;
    while(1) {
        if (in(0x03fd) % 2) {
            unsigned char inByte = in(0x03f8);
            if (inByte == '\r') {
                buffer[index] = 0;
                out(0x03f8, '\r');
                out(0x03f8, '\n');
                return;
            }
            else if (inByte == '\b') {
                if (index > 0) {
                    index--;
                    out(0x03f8, '\b');
                    out(0x03f8, ' ');
                    out(0x03f8, '\b');
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

void print(char* string) {
    for (int i = 0; string[i] != 0; i++) {
        out(0x03f8, string[i]);
    }
}