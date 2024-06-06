#include "fat16.h"
#include "ata.h"

Bpb readbpb(unsigned char controller, unsigned char driveNumber, unsigned int lba) {
    Bpb bpb;
    unsigned char partLba[6] = {0};
    partLba[0] = lba & 0x000000ff;
    partLba[1] = (lba & 0x0000ff00) >> 8;
    partLba[2] = (lba & 0x00ff0000) >> 16;
    partLba[3] = (lba & 0xff000000) >> 24;
    ata_read(controller, driveNumber, partLba, 1, &bpb);
    return bpb;
}

void listRootDir(unsigned char controller, unsigned char driveNumber, unsigned int lba, RootDirEntry* entries) {
    Bpb bpb = readbpb(controller, driveNumber, lba);
    unsigned short root_dir_sectors = ((bpb.numRootDirEntries * 32) + (bpb.bytesPerSector - 1)) / bpb.bytesPerSector;
    unsigned short root_dir_start = bpb.reservedSectors + (bpb.numFats * bpb.sectorsPerFat);
    unsigned char rootDirLba[6] = {0};
    rootDirLba[0] = (lba + root_dir_start) & 0x000000ff;
    rootDirLba[1] = ((lba + root_dir_start) & 0x0000ff00) >> 8;
    rootDirLba[2] = ((lba + root_dir_start) & 0x00ff0000) >> 16;
    rootDirLba[3] = ((lba + root_dir_start) & 0xff000000) >> 24;
    ata_read(controller, driveNumber, rootDirLba, root_dir_sectors, entries);
}