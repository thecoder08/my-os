typedef struct _guiapp {
    struct _guiapp* next;
    int x;
    int y;
    Framebuffer fb;
    int id;
} GuiApp;

void initGui();
int initWindow(int width, int height, const char* title);
void updateWindow(int appId, Framebuffer back);