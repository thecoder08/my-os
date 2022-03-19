#include "io.h"

static unsigned char buffer[100];

char* scan() {
    int index = 0;
    while(1) {
        if (in(0x03fd) % 2) {
            unsigned char inByte = in(0x03f8);
            if (inByte == '\r') {
                buffer[index] = '\r';
                buffer[index + 1] = '\n';
                buffer[index + 2] = 0;
                out(0x03f8, '\r');
                out(0x03f8, '\n');
                return buffer;
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