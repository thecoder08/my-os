#include "graphics.h"
#include "mem.h"
#include "gui.h"

#define WIDTH 480
#define HEIGHT 240

static Framebuffer backbuffer;
extern int smallFont;

void writeCharTerm(int x, int y, unsigned char character) {
    if (character > 127) {
        return;
    }
    bufferStruct(x*6, y*10, 6, 10, &smallFont + character*60, backbuffer);
}

void printTerm(const char* string) {
    for (int i = 0; string[i] != 0; i++) {
        writeCharTerm(i, 0, string[i]);
    }
}

void initTerm() {
    int id = initWindow(WIDTH, HEIGHT, "Terminal");
    backbuffer.width = WIDTH,
    backbuffer.height = HEIGHT,
    backbuffer.data = malloc(WIDTH*HEIGHT*4);
    printTerm("Hello World!");
    updateWindow(id, backbuffer);

    while(1) {
        asm("int $0x20");
    }
}