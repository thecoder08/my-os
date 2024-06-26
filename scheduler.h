typedef struct {
    void* entryPointer;
    void* stackPointer;
} Process;
void registerProcess(void* entryPoint);