#include "ps2.h"
#include "mouse.h"

char bytePosition = -1;
unsigned char flagByte;
unsigned char xByte;
unsigned char yByte;

void (*mouseData)(unsigned char flags, unsigned char x, unsigned char y);

void mouseHandler(unsigned char byte) {
    if (bytePosition == -1 && byte == 0xfa) {
        bytePosition = 0;
        return;
    }
    if (bytePosition == 0) {
        flagByte = byte;
        bytePosition = 1;
        return;
    }
    if (bytePosition == 1) {
        xByte = byte;
        bytePosition = 2;
        return;
    }
    if (bytePosition == 2) {
        yByte = byte;
        bytePosition = 0;
        mouseData(flagByte, xByte, yByte);
        return;
    }
}

void initMouse(void (*mouseDataHandler)(unsigned char flags, unsigned char x, unsigned char y)) {
    mouseData = mouseDataHandler;
    // enable data reporting
    write_ps2_2(0xf4);
    init_ps2_2(mouseHandler);
}