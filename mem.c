#include "mem.h"

// the heap is just a pointer to some memory. Malloc increases it and returns the new result.
void* heap = (void*) 0x00100000; // Heap starts at beginning of high memory so we are out of the way.

void* malloc(int bytes) {
    void* result = heap;
    heap += bytes;
    return result;
}

void memcpy(char* source, char* dest, int length) {
    register char* s asm("esi") = source;
    register char* d asm("edi") = dest;
    register int l asm("ecx") = length;
    asm("rep movsb" : : ""(s), ""(d), ""(l));
}

void memcpyDwords(int* source, int* dest, int length) {
    register int* s asm("esi") = source;
    register int* d asm("edi") = dest;
    register int l asm("ecx") = length;
    asm("rep movsd" : : ""(s), ""(d), ""(l));
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