#include "interrupts.h"
#include "terminal.h"
#include "io.h"

IdtEntry idt[256];
IdtDescriptor idtDescriptor;

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

#define ICW1_ICW4	0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */
 
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */
#define PIC_EOI 0x20

void sendEOI(unsigned char irq) {
    if(irq >= 8)
		out(PIC2_COMMAND,PIC_EOI);
 
	out(PIC1_COMMAND,PIC_EOI);
}

__attribute__((interrupt)) void isr0Handler(struct interrupt_frame* frame) {
    print("Division by zero exception!\r\n");
    while(1);
}

__attribute__((interrupt)) void isr1Handler(struct interrupt_frame* frame) {
    print("Debug exception!\r\n");
    while(1);
}

__attribute__((interrupt)) void isr2Handler(struct interrupt_frame* frame) {
    print("Non-maskable interrupt!\r\n");
    while(1);
}

__attribute__((interrupt)) void isr3Handler(struct interrupt_frame* frame) {
    print("Breakpoint exception!\r\n");
    while(1);
}

__attribute__((interrupt)) void isr4Handler(struct interrupt_frame* frame) {
    print("Overflow exception!\r\n");
    while(1);
}

__attribute__((interrupt)) void isr5Handler(struct interrupt_frame* frame) {
    print("Bound range exceeded exception!\r\n");
    while(1);
}

__attribute__((interrupt)) void isr6Handler(struct interrupt_frame* frame) {
    print("Invalid opcode exception!\r\n");
    while(1);
}

__attribute__((interrupt)) void isr7Handler(struct interrupt_frame* frame) {
    print("Device not available exception!\r\n");
    while(1);
}

__attribute__((interrupt)) void isr8Handler(struct interrupt_frame* frame) {
    print("Double fault exception!\r\n");
    while(1);
}

__attribute__((interrupt)) void isr9Handler(struct interrupt_frame* frame) {
    print("Coprocessor segment overrun exception!\r\n");
    while(1);
}

__attribute__((interrupt)) void isr10Handler(struct interrupt_frame* frame) {
    print("Invalid TSS exception!\r\n");
    while(1);
}

__attribute__((interrupt)) void isr11Handler(struct interrupt_frame* frame) {
    print("Segment not present exception!\r\n");
    while(1);
}

__attribute__((interrupt)) void isr12Handler(struct interrupt_frame* frame) {
    print("Stack segment fault exception!\r\n");
    while(1);
}

__attribute__((interrupt)) void isr13Handler(struct interrupt_frame* frame) {
    print("General protection fault exception!\r\n");
    while(1);
}

__attribute__((interrupt)) void isr14Handler(struct interrupt_frame* frame) {
    print("Page fault exception!\r\n");
    while(1);
}

__attribute__((interrupt)) void isr16Handler(struct interrupt_frame* frame) {
    print("x87 floating-point exception!\r\n");
    while(1);
}

__attribute__((interrupt)) void isr17Handler(struct interrupt_frame* frame) {
    print("Alignment check exception!\r\n");
    while(1);
}

__attribute__((interrupt)) void isr18Handler(struct interrupt_frame* frame) {
    print("Machine check exception!\r\n");
    while(1);
}

__attribute__((interrupt)) void isr19Handler(struct interrupt_frame* frame) {
    print("SIMD Floating-Point exception!\r\n");
    while(1);
}

__attribute__((interrupt)) void isr20Handler(struct interrupt_frame* frame) {
    print("Virtualisation exception!\r\n");
    while(1);
}

__attribute__((interrupt)) void isr21Handler(struct interrupt_frame* frame) {
    print("Control protection exception!\r\n");
    while(1);
}

__attribute__((interrupt)) void isr28Handler(struct interrupt_frame* frame) {
    print("Hypervisor Injection exception!\r\n");
    while(1);
}

__attribute__((interrupt)) void isr29Handler(struct interrupt_frame* frame) {
    print("VMM Communication exception!\r\n");
    while(1);
}

__attribute__((interrupt)) void isr30Handler(struct interrupt_frame* frame) {
    print("Security exception!\r\n");
    while(1);
}

__attribute__((interrupt)) void reservedHandler(struct interrupt_frame* frame) {
    print("Reserved exception!\r\n");
    while(1);
}

__attribute__((interrupt)) void irqHandler(struct interrupt_frame* frame) {
    print("Got an IRQ!\r\n");
    for (unsigned char i = 0; i < 16; i++) {
        sendEOI(i);
    }
}

void registerIsr(unsigned char entry, void (*isrHandler) (struct interrupt_frame* frame)) {
    idt[entry].lowOffset = low_16((unsigned int)isrHandler);
    idt[entry].selector = 0x08; // code segment
    idt[entry].always0 = 0;
    idt[entry].flags = 0x8E;
    idt[entry].highOffset = high_16((unsigned int)isrHandler);
}

/* reinitialize the PIC controllers, giving them specified vector offsets
   rather than 8h and 70h, as configured by default */
 
/*
arguments:
	offset1 - vector offset for master PIC
		vectors on the master become offset1..offset1+7
	offset2 - same for slave PIC: offset2..offset2+7
*/

void io_wait() {
    for (int i = 0; i < 256; i++);
}

void PIC_remap(int offset1, int offset2) {
	unsigned char a1, a2;
 
	a1 = in(PIC1_DATA);                        // save masks
	a2 = in(PIC2_DATA);
 
	out(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	out(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	out(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	out(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	out(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	out(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
 
	out(PIC1_DATA, ICW4_8086);               // ICW4: have the PICs use 8086 mode (and not 8080 mode)
	io_wait();
	out(PIC2_DATA, ICW4_8086);
	io_wait();
 
	out(PIC1_DATA, a1);   // restore saved masks.
	out(PIC2_DATA, a2);
}

void initializeIdt() {
    // fill in idt entries
    registerIsr(0, isr0Handler);
    registerIsr(1, isr1Handler);
    registerIsr(2, isr2Handler);
    registerIsr(3, isr3Handler);
    registerIsr(4, isr4Handler);
    registerIsr(5, isr5Handler);
    registerIsr(6, isr6Handler);
    registerIsr(7, isr7Handler);
    registerIsr(8, isr8Handler);
    registerIsr(9, isr9Handler);
    registerIsr(10, isr10Handler);
    registerIsr(11, isr11Handler);
    registerIsr(12, isr12Handler);
    registerIsr(13, isr13Handler);
    registerIsr(14, isr14Handler);
    registerIsr(15, reservedHandler);
    registerIsr(16, isr16Handler);
    registerIsr(17, isr17Handler);
    registerIsr(18, isr18Handler);
    registerIsr(19, isr19Handler);
    registerIsr(20, isr20Handler);
    registerIsr(21, isr21Handler);
    registerIsr(22, reservedHandler);
    registerIsr(23, reservedHandler);
    registerIsr(24, reservedHandler);
    registerIsr(25, reservedHandler);
    registerIsr(26, reservedHandler);
    registerIsr(27, reservedHandler);
    registerIsr(28, isr28Handler);
    registerIsr(29, isr29Handler);
    registerIsr(30, isr30Handler);
    registerIsr(31, reservedHandler);
    // create the idt descriptor and set the size and address
    idtDescriptor.size = sizeof(idt) - 1;
    idtDescriptor.idtAddress = idt;
    // remap the PIC, irqs 0-7 -> isrs 32-39, irqs 8-15 -> isrs 40-47
    PIC_remap(32, 40);
    // add irq handlers
    for (unsigned char i = 32; i < 48; i++) {
        registerIsr(i, irqHandler);
    }
    // load the address of the idt descriptor
    asm("lidtl (%0)" : : "r" (&idtDescriptor));
    asm("sti");
    print("Loaded IDT\r\n");
}

void addIrqHandler(unsigned char irq, void (*irqHandler) (struct interrupt_frame* frame)) {
    asm("cli");
    registerIsr(irq + 32, irqHandler);
    asm("lidtl (%0)" : : "r" (&idtDescriptor));
    asm("sti");
}

