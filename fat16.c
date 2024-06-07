#include "fat16.h"
#include "ata.h"
#include "string.h"
#include "terminal.h"
#include "mem.h"

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

unsigned int clusterToSector(unsigned int cluster, Bpb bpb, unsigned int lba) {
    unsigned int cluster2address = lba + (bpb.numHiddenSectors*bpb.bytesPerSector + bpb.sectorsPerFat*bpb.bytesPerSector*bpb.numFats + bpb.numRootDirEntries*32); // location on disk of cluster #2 (first data cluster)
    unsigned int clusterAddress = cluster2address + (cluster-2)*bpb.sectorsPerCluster*bpb.bytesPerSector;
    return clusterAddress / 512; // assumes multiple of 512
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

// reads a file from the specified disk, with the specified name and extension, and the specified partition-start lba (given by reading the partition table)
int readFile(unsigned char controller, unsigned char driveNumber, char* name, char* extension, unsigned int lba, void* buffer) {
    static RootDirEntry entries[1000];
    listRootDir(controller, driveNumber, lba, entries);
    for (int i = 0; entries[i].name[0] != 0; i++) {
        // strings must be null-terminated
        char nameStr[9] = {0};
        memcpy(entries[i].name, nameStr, 8);
        char extStr[4] = {0};
        memcpy(entries[i].extension, extStr, 3);
        if (strcmp(name, nameStr) && strcmp(extension, extStr)) {
            Bpb bpb = readbpb(controller, driveNumber, lba);
            unsigned int readSector = clusterToSector(entries[i].cluster, bpb, lba);
            unsigned char partLba[6] = {0};
            partLba[0] = readSector & 0x000000ff;
            partLba[1] = (readSector & 0x0000ff00) >> 8;
            partLba[2] = (readSector & 0x00ff0000) >> 16;
            partLba[3] = (readSector & 0xff000000) >> 24;
            return ata_read(controller, driveNumber, partLba, entries[i].size / 512 + 1, buffer);
        }
    }
    print("File not found\r\n");
    return 1;
}