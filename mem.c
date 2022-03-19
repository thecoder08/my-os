// the heap is just a pointer to some memory. Malloc increases it and returns the new result.

char* heap = (char*) 0x00010000;

char* malloc(int bytes) {
    heap += bytes;
    return heap;
}
void memcpy(char* source, char* dest, int length) {
    for (int i = 0; i < length; i++) {
        dest[i] = source[i];
    }
}

void memset(char* dest, char val, int length) {
    for (int i = 0; i < length; i++) {
        *(dest + i) = val;
    }
}