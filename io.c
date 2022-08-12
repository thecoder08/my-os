#include "io.h"

unsigned char in(unsigned short port) {
    unsigned char result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void out(unsigned short port, unsigned char data) {h
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}