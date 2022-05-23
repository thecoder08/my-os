#include "io.h"
#include "vga.h"

char* mapping_default = "??1234567890-=??qwertyuiop[]??asdfghjkl;'`?\\zxcvbnm,./??? ???????????????????????????????";
char* mapping_shifted = "??!@#$%^&*()_+??QWERTYUIOP{}??ASDFGHJKL:\"~?|ZXCVBNM<>???? ???????????????????????????????";

void keyboardScan(char* buffer) {
    int shifted = 0;
    int i = 0;
    while(1) {
        if (in(0x64) % 2) {
            unsigned char inbyte = in(0x60);
            if (inbyte > 0x80) {
                if ((inbyte == 0xaa) || (inbyte == 0xb6)) {
                    shifted = 0;
                }
            }
            else {
                if ((inbyte == 0x2a) || (inbyte == 0x36)) {
                    shifted = 1;
                }
                else if (inbyte == 0x1c) {
                    vgaPrint("\r\n");
                    buffer[i] = 0;
                    return;
                }
                else if (inbyte == 0x0e) {
                    if (i > 0) {
                        i--;
                        writeChar('\b');
                        writeChar(' ');
                        writeChar('\b');
                    }
                }
                else {
                    if (shifted) {
                        buffer[i] = mapping_shifted[inbyte];
                    }
                    else {
                        buffer[i] = mapping_default[inbyte];
                    }
                    writeChar(buffer[i]);
                    i++;
                }
            }
        }
    }
}