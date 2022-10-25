void ata_write(int controller, int drive, int lba, int size, unsigned char* buffer);
void ata_read(int controller, int drive, int lba, int size, unsigned char* buffer);