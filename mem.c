#include "mem.h"

// the heap is just a pointer to some memory. Malloc increases it and returns the new result.
char* heap = (char*) 0x00040000;

char* malloc(int bytes) {
    char* result = heap;
    heap += bytes;
    return result;
}

void memcpy(char* source, char* dest, int length) {
    for (int i = 0; i < length; i++) {
        dest[i] = source[i];
    }
}

void memset(char* dest, char val, int length) {
    for (int i = 0; i < length; i++) {
        dest[i] = val;
    }
}

int memcmp(char* string1, char* string2, int length) {
    for (int i = 0; i < length; i++) {
        if (string1[i] != string2[i]) {
            return 0;
        }
    }
    return 1;
}