#include "interrupts.h"
#include "terminal.h"

void isrHandler() {
    print("My man literally divided by zero\r\n");
}

void initializeIdt() {
    // create the idt and fill in an entry
    IdtEntry idt[256];
    idt[0].lowOffset = low_16((unsigned int)isrHandler);
    idt[0].selector = 0x08; // idk what this means
    idt[0].always0 = 0;
    idt[0].flags = 0x8E;
    idt[0].highOffset = high_16((unsigned int)isrHandler);
    // create the idt descriptor and set the size and address
    IdtDescriptor idtDescriptor;
    idtDescriptor.size = sizeof(idt) - 1;
    idtDescriptor.idtAddress = idt;
    // load the address of the idt descriptor
    asm("lidtl (%0)" : : "r" (&idtDescriptor));
}

