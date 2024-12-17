#include "graphics.h"
#include "gui.h"
#include "mouse.h"
#include "mem.h"

GuiApp* apps; // head
int newAppId = 0;
GuiApp* draggedApp;
int dragging = 0;

int x = 0;
int y = 0;

extern int cursorImage;
extern int fingerImage;

extern Framebuffer image;
Framebuffer backbuffer;

int oldMousedown = 0;
int mousedown = 0;

int checkOverlap(int left1, int right1, int top1, int bottom1, int left2, int right2, int top2, int bottom2) {
    return left1 <= right2 && right1 >= left2 && top1 <= bottom2 && bottom1 >= top2;
}

int initWindow(int width, int height, const char* title) {
    GuiApp* newApp = malloc(sizeof(GuiApp));
    newApp->x = 0;
    newApp->y = 0;
    newApp->next = 0;
    newApp->fb.width = width;
    newApp->fb.height = height;
    newApp->fb.data = malloc(width*height*4);
    newApp->id = newAppId;
    newAppId++;
    if (apps == 0) {
        apps = newApp;
        return newApp->id;
    }
    GuiApp* app = apps;
    while(app->next != 0) {
        app = app->next;
    }
    app->next = newApp;
    return newApp->id;
}

GuiApp* findAppById(int appId) {
    GuiApp* app = apps;
    while(app->id != appId && app->next != 0) {
        app = app->next;
    }
    return app;
}

int shouldRedraw = 1;
void mouse(unsigned char flags, short deltaX, short deltaY) {
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
    mousedown = (flags & 0x01);
    if (mousedown && !oldMousedown) {
        draggedApp = apps;
        dragging = 0;
        while (draggedApp != 0) {
            if (checkOverlap(draggedApp->x, draggedApp->x + draggedApp->fb.width, draggedApp->y, draggedApp->y + draggedApp->fb.height, x, x+20, y, y+20)) {
                dragging = 1;
                break;
            }
            else {
                draggedApp = draggedApp->next;
            }
        }
    }
    oldMousedown = mousedown;
    shouldRedraw = 1;
}

void updateWindow(int appId, Framebuffer back) {
    GuiApp* app = findAppById(appId);
    memcpyDwords(back.data, app->fb.data, back.width*back.height);
    shouldRedraw = 1;
}

void initGui() {
    initMouse(mouse);
    backbuffer.height = image.height;
    backbuffer.width = image.width;
    backbuffer.data = malloc(image.height*image.width*4);
    while(1) {
        if (shouldRedraw) {
            // clear
            rectangleStruct(0, 0, backbuffer.width, backbuffer.height, 0xff005555, backbuffer);
            // draw apps in order
            GuiApp* app = apps;
            while(app != 0) {
                bufferStruct(app->x, app->y, app->fb.width, app->fb.height, app->fb.data, backbuffer);
                app = app->next;
            }
            // draw mouse
            if (mousedown) {
                alphaBufferStruct(x, y, 20, 20, &fingerImage, backbuffer);
                if (dragging) {
                    draggedApp->x = x;
                    draggedApp->y = y;
                }
            }
            else {
                alphaBufferStruct(x, y, 20, 20, &cursorImage, backbuffer);
            }
            // update display
            memcpyDwords(backbuffer.data, image.data, image.height*image.width);
            shouldRedraw = 0;
        }
        asm("int $0x20");
    }
}