#include "keyboard.h"
#include "vga.h"
#include "serial.h"
#include "terminal.h"

int tty = 1;

void scan(char* buffer) {
    if (tty) {
        keyboardScan(buffer);
    }
    else {
        serialScan(buffer);
    }
}

void print(char* string) {
    if (tty) {
        vgaPrint(string);
    }
    else {
        serialPrint(string);
    }
}

void ctty(int newTty) {
    tty = newTty;
}