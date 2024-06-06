#include "gdt.h"
#include "terminal.h"

GdtEntry gdt[3];
GdtDescriptor gdtDescriptor;

void new_gdt();

void initializeGdt() {
    // null segment
    gdt[0].limitLow = 0;
    gdt[0].baseLow = 0;
    gdt[0].baseMid = 0;
    gdt[0].access = 0;
    gdt[0].flagsLimitHigh = 0;
    gdt[0].baseHigh = 0;
    // code segment
    gdt[1].limitLow = 0xFFFF;
    gdt[1].baseLow = 0;
    gdt[1].baseMid = 0;
    gdt[1].access = 0x9A;
    gdt[1].flagsLimitHigh = 0xCF;
    gdt[1].baseHigh = 0;
    // data segment
    gdt[2].limitLow = 0xFFFF;
    gdt[2].baseLow = 0;
    gdt[2].baseMid = 0;
    gdt[2].access = 0x92;
    gdt[2].flagsLimitHigh = 0xCF;
    gdt[2].baseHigh = 0;
    // gdt descriptor
    
    gdtDescriptor.size = sizeof(gdt) - 1;
    gdtDescriptor.gdtAddress = gdt;
    
    // load/enter our gdt
    new_gdt();
}