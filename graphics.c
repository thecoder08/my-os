#include "graphics.h"

Framebuffer image;

void setFb(Framebuffer fb) {
    image = fb;
}

void plot(int x, int y, int color) {
  if (x >= 0 && x < image.width && y >= 0 && y < image.height) {
    image.data[y*image.width + x] = color;
  }
}

void circle(int x, int y, int radius, int color) {
  for (int i = -radius; i < radius; i++) {
    for (int j = -radius; j < radius; j++) {
      if (j*j + i*i < radius*radius) {
        plot(x + j, y + i, color);
      }
    }
  }
}

void rectangle(int x, int y, int width, int height, int color) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      plot(x + j, y + i, color);
    }
  }
}

void drawBuffer(int x, int y, int width, int height, int* source) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      plot(x + j, y + i, source[i * width + j]);
    }
  }
}