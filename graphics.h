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