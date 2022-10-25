#include "io.h"

unsigned char in(unsigned short port) {
    unsigned char result;
    asm("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void out(unsigned short port, unsigned char data) {
    asm("out %%al, %%dx" : : "a" (data), "d" (port));
}