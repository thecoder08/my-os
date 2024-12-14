#include "gui.h"
#include "graphics.h"
#include "mouse.h"
#include "mem.h"

typedef struct _guiapp {
    struct _guiapp* next;
    int x;
    int y;
    int width;
    int height;
} GuiApp;

GuiApp* apps; // head

int x = 0;
int y = 0;

extern int cursorImage;
extern int fingerImage;

extern Framebuffer image;

int backbuffer[20*20];

void loadBackBuffer() {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            backbuffer[i*20 + j] = image.data[(y+i)*image.width + x + j];
        }
    }
}

void mouse(unsigned char flags, short deltaX, short deltaY) {
    drawBuffer(x, y, 20, 20, backbuffer);
    x += deltaX;
    if (x < 0) {
        x = 0;
    }
    if (x >= image.width) {
        x = image.width - 1;
    }
    y += deltaY;
    if (y < 0) {
        y = 0;
    }
    if (y >= image.height) {
        y = image.height - 1;
    }
    loadBackBuffer();
    if (flags & 0x01) {
        alphaBuffer(x, y, 20, 20, &fingerImage);
    }
    else {
        alphaBuffer(x, y, 20, 20, &cursorImage);
    }
}

void initWindow(int width, int height, const char* title) {
    GuiApp* newApp = malloc(sizeof(GuiApp));
    newApp->width = width;
    newApp->height = height;
    newApp->x = 0;
    newApp->y = 0;
    newApp->next = 0;
    GuiApp* app = apps;
    while(app->next != 0) {
        app = app->next;
    }
    app->next = newApp;
}

void initGui() {
    initMouse(mouse);
}