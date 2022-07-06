#include "io.h"

unsigned char read_ps2_1() {
    while(1) {
        if (in(0x64) % 2) {
            return in(0x60);
        }
    }
}
unsigned char read_ps2_2() {
    while(1) {
        if (in(0x64) % 2) {
            return in(0x60);
        }
    }
}
void write_ps2_1(unsigned char byte) {
    out(0x60, byte);
}
void write_ps2_2(unsigned char byte) {
    out(0x64, 0xd4);
    out(0x60, byte);
}