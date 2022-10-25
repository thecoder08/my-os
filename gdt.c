#include "gdt.h"

void initializeGdt() {
    GdtEntry gdt[4];
    gdt[0] = 0;
    GdtDescriptor gdtDescriptor;
    gdtDescriptor.size = sizeof(gdt) - 1;
    gdtDescriptor.gdtAddress = gdt;
    asm("lgdtl (%0)" : : "r" (&gdtDescriptor));
}