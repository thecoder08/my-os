#include "ata.h"

// parameters: controller - 0 for primary, 1 for secondary. drive - 0 for master, 1 for slave. lba - position to read/write. size - number of bytes to read/write. buffer - pointer to buffer to read/write
void ata_write(int controller, int drive, int lba, int size, unsigned char* buffer) {

}

void ata_read(int controller, int drive, int lba, int size, unsigned char* buffer) {

}