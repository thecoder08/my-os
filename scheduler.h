typedef struct _process {
    void* stackPointer;
    struct _process* next;
    int pid;
} Process;

int registerProcess(void* entryPoint);
void initScheduler();