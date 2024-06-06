#include "io.h"

unsigned char in(unsigned short port) {
    unsigned char result;
    asm("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void out(unsigned short port, unsigned char data) {
    asm("out %%al, %%dx" : : "a" (data), "d" (port));
}

unsigned short ins(unsigned short port) {
    unsigned short result;
    asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void outs(unsigned short port, unsigned short data) {
    asm("out %%ax, %%dx" : : "a" (data), "d" (port));
}