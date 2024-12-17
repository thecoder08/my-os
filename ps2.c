#include "io.h"
#include "ps2.h"
#include "interrupts.h"
#include "terminal.h"

void write_ps2_1(unsigned char byte) {
    while(in(0x64) & 0x2);
    out(0x60, byte);
}
void write_ps2_2(unsigned char byte) {
    out(0x64, 0xd4);
    while(in(0x64) & 0x2);
    out(0x60, byte);
}

void (*ps2_1_data)(unsigned char byte);
void (*ps2_2_data)(unsigned char byte);

__attribute__((interrupt)) void ps2_1_handler(struct interrupt_frame* frame) {
    ps2_1_data(in(0x60));
    sendEOI(1);
}

__attribute__((interrupt)) void ps2_2_handler(struct interrupt_frame* frame) {
    ps2_2_data(in(0x60));
    sendEOI(12);
}

void init_ps2_1(void (*ps2_1_data_handler)(unsigned char byte)) {
    out(0x64, 0xae);
    out(0x64, 0x60);
    while(in(0x64) & 0x2);
    out(0x60, 0x43);
    ps2_1_data = ps2_1_data_handler;
    addIrqHandler(1, ps2_1_handler);
}

void init_ps2_2(void (*ps2_2_data_handler)(unsigned char byte)) {
    out(0x64, 0xa8);
    out(0x64, 0x60);
    while(in(0x64) & 0x2);
    out(0x60, 0x43);
    ps2_2_data = ps2_2_data_handler;
    addIrqHandler(12, ps2_2_handler);
}