#include "timer.h"
#include "string.h"
#include "interrupts.h"
#include "terminal.h"
#include "io.h"

void ctxsw();

void init_timer(unsigned int freq) {
    /* Install the handler function */
    addIrqHandler(0, ctxsw);
    
    /* Get the PIT value: hardware clock at 1193182 Hz */
    unsigned int divisor = 1193182 / freq;
    unsigned char low  = (unsigned char)(divisor & 0xFF);
    unsigned char high = (unsigned char)((divisor >> 8) & 0xFF);
    /* Send the command */
    out(0x43, 0x34); /* Command port */
    out(0x40, low);
    out(0x40, high);
}