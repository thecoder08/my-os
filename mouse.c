#include "ps2.h"
#include "mouse.h"

void mouseScan(unsigned char* flags, unsigned char* x, unsigned char* y) {
    // enable data reporting
    write_ps2_2(0xf4);
    // wait for acknoledgement
    if (read_ps2_2() == 0xfa) {
        // read data
        *flags = read_ps2_2();
        *x = read_ps2_2();
        *y = read_ps2_2();
    }
}