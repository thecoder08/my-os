#include "timer.h"
#include "string.h"
#include "interrupts.h"
#include "terminal.h"
#include "io.h"

void contextSwitch();

__attribute__((interrupt)) void timerPlaceholder(struct interrupt_frame* frame) {
    sendEOI(0);
}

void init_timer(unsigned int freq) {
    /* Install the function we just wrote */
    addIrqHandler(0, timerPlaceholder);

    /* Get the PIT value: hardware clock at 1193180 Hz */
    unsigned int divisor = 1193180 / freq;
    unsigned char low  = (unsigned char)(divisor & 0xFF);
    unsigned char high = (unsigned char)( (divisor >> 8) & 0xFF);
    /* Send the command */
    out(0x43, 0x36); /* Command port */
    out(0x40, low);
    out(0x40, high);
}
