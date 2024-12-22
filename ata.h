typedef struct {
    unsigned char primaryMaster;
    unsigned char primarySlave;
    unsigned char secondaryMaster;
    unsigned char secondarySlave;
} Drives;

Drives detect_drives();
int ata_write(unsigned char controller, unsigned char driveNumber, unsigned int lba, unsigned char sectors, void* buffer);
int ata_read(unsigned char controller, unsigned char driveNumber, unsigned int lba, unsigned char sectors, void* buffer);
int ata_write28(unsigned char controller, unsigned char driveNumber, unsigned char* lba, unsigned short sectors, void* buffer);
int ata_read28(unsigned char controller, unsigned char driveNumber, unsigned char* lba, unsigned short sectors, void* buffer);
void reset(unsigned short base);