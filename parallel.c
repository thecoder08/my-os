#include "io.h"
#include "parallel.h"

void parallelSend(unsigned char byte) {
    out(0x378, byte);
    unsigned char control = in(0x37a);
    out(0x37a, control | 1);
    out(0x37a, control);
}