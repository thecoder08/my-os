typedef struct {
    int width;
    int height;
    int* data;
} Framebuffer;

void setFb(Framebuffer fb);
void clear();
void plot(int x, int y, int color);
void line(int x0, int y0, int x1, int y1, int color);
void circle(int x, int y, int radius, int color);
void rectangle(int x, int y, int width, int height, int color);
void drawBuffer(int x, int y, int width, int height, int* source);
void alphaBuffer(int x, int y, int width, int height, int* source);
void plotStruct(int x, int y, int color, Framebuffer fb);
void clearStruct(Framebuffer fb);
void rectangleStruct(int x, int y, int width, int height, int color, Framebuffer fb);
void circleStruct(int x, int y, int radius, int color, Framebuffer fb);
void lineStruct(int x0, int y0, int x1, int y1, int color, Framebuffer fb);
void bufferStruct(int x, int y, int width, int height, int* source, Framebuffer fb);
void alphaBufferStruct(int x, int y, int width, int height, int* source, Framebuffer fb);