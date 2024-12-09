typedef struct {
    void* stackPointer;
    Process* next;
} Process;

void registerProcess(void* entryPoint);