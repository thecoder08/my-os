#include "io.h"
#include "ps2.h"
#include "string.h"
#include "terminal.h"

unsigned char read_ps2_1() {
        if (in(0x64) & 0x1) {
            return in(0x60);
        }
        else {
            return 0;
        }
}
unsigned char read_ps2_2() {
        if (in(0x64) & 0x1) {
            return in(0x60);
        }
        else {
            return 0;
        }
}
void write_ps2_1(unsigned char byte) {
    while(in(0x64) & 0x2);
    out(0x60, byte);
}
void write_ps2_2(unsigned char byte) {
    while(in(0x64) & 0x2);
    out(0x64, 0xd4);
    out(0x60, byte);
}