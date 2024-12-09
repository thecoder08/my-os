typedef struct _process {
    void* stackPointer;
    struct _process* next;
} Process;

void registerProcess(void* entryPoint);
