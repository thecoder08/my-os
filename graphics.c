#include "graphics.h"
#include "mem.h"

Framebuffer image;

void setFb(Framebuffer fb) {
    image = fb;
}

void plot(int x, int y, int color) {
  plotStruct(x, y, color, image);
}

void clear() {
  clearStruct(image);
}

void rectangle(int x, int y, int width, int height, int color) {
  rectangleStruct(x, y, width, height, color, image);
}

void circle(int x, int y, int radius, int color) {
  circleStruct(x, y, radius, color, image);
}

int abs(int x) { return x > 0 ? x : -x; }

void line(int x0, int y0, int x1, int y1, int color) {
   lineStruct(x0, y0, x1, y1, color, image);
}

void drawBuffer(int x, int y, int width, int height, int* source) {
  bufferStruct(x, y, width, height, source, image);
}

void alphaBuffer(int x, int y, int width, int height, int* source) {
  alphaBufferStruct(x, y, width, height, source, image);
}

void plotStruct(int x, int y, int color, Framebuffer fb) {
  if (x >= 0 && x < fb.width && y >= 0 && y < fb.height) {
    fb.data[y*fb.width + x] = color;
  }
}

void clearStruct(Framebuffer fb) {
  memset((char*)fb.data, 0, fb.height*fb.width*4);
}

void rectangleStruct(int x, int y, int width, int height, int color, Framebuffer fb) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      plotStruct(x + j, y + i, color, fb);
    }
  }
}

void circleStruct(int x, int y, int radius, int color, Framebuffer fb) {
  for (int i = -radius; i < radius; i++) {
    for (int j = -radius; j < radius; j++) {
      if (j*j + i*i < radius*radius) {
        plotStruct(x + j, y + i, color, fb);
      }
    }
  }
}

void lineStruct(int x0, int y0, int x1, int y1, int color, Framebuffer fb) {
   int dx = abs(x1 - x0);
   int dy = abs(y1 - y0);
   int sx = (x0 < x1) ? 1 : -1;
   int sy = (y0 < y1) ? 1 : -1;
   int err = dx - dy;
   while(1) {
      plotStruct(x0, y0, color, fb);
      if ((x0 == x1) && (y0 == y1)) break;
      int e2 = 2*err;
      if (e2 > -dy) { err -= dy; x0  += sx; }
      if (e2 < dx) { err += dx; y0  += sy; }
   }
}

void bufferStruct(int x, int y, int width, int height, int* source, Framebuffer fb) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      plotStruct(x + j, y + i, source[i * width + j], fb);
    }
  }
}

void alphaBufferStruct(int x, int y, int width, int height, int* source, Framebuffer fb) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if ((source[i * width + j] & 0xff000000) == 0xff000000) {
        plotStruct(x + j, y + i, source[i * width + j], fb);
      }
    }
  }
}