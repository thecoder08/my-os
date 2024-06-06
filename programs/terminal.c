#include "../string.h"
#include "../terminal.h"

int read(int fd, void* buffer, int size);
int write(int fd, void* buffer, int size);

void scan(char* buffer) {
    int i = 0;
    do {
        read(0, buffer + i, 1);
        i++;
    } while (buffer[i-1] != '\n');
    buffer[i-1] = 0;
}

void print(char* string) {
    write(1, string, strlen(string));
}
