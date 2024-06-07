#include "interrupts.h"
#include "terminal.h"
#include "string.h"

__attribute__((interrupt)) void syscall(struct interrupt_frame* frame) {
    asm("sti");
    int opcode;
    asm("\t movl %%ecx,%0" : "=r"(opcode));
    char* param;
    asm("\t movl %%edx,%0" : "=r"(param));
    switch(opcode) {
        case 0:
            print(param);
            break;
        case 1:
            scan(param);
            break;
    }
}

void initSyscall() {
    registerIsr(0x80, syscall);
}