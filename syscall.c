#include "interrupts.h"
#include "terminal.h"
#include "string.h"
#include "fat16.h"
#include "parttable.h"
#include "mem.h"

extern RootDirEntry rootDir[1000]; // doing this to save memory

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
        case 2:
            PartTableEntry entries[4];
            readPartitionTable(0, 0, entries);
            if (entries[0].type == 4) { // if first partition is fat16-formatted, try loading and running 
                char name[9] = {0};
                char extension[4] = {0};
                memcpy(param, name, 8);
                memcpy(param+8, extension, 3);
                int readStatus = readFile(0, 0, name, extension, entries[0].lba, (void*) 0x30000);
                if (readStatus == 0) {
                    asm("call $0x08,$0x30000");
                    asm("mov $0,%esi");
                    return;
                }
                else {
                    asm("mov $1,%esi");
                    return;
                }
            }
            break;
        case 3:
            readPartitionTable(0, 0, entries);
            listRootDir(0, 0, entries[0].lba, rootDir);
            print("Root directory contents:\r\n");
            for (int i = 0; rootDir[i].name[0] != 0; i++) {
                if (rootDir[i].attributes == 0x0f) {
                    continue;
                }
                // strings must be null-terminated
                char nameStr[9] = {0};
                memcpy(rootDir[i].name, nameStr, 8);
                char extStr[4] = {0};
                memcpy(rootDir[i].extension, extStr, 3);
                print(nameStr);
                print(extStr);
                print("\r\n");
            }
            break;
    }
}

void initSyscall() {
    registerIsr(0x80, syscall);
}