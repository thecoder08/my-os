#include "fat16.h"
#include "ata.h"
#include "string.h"
#include "terminal.h"
#include "mem.h"

DirEntry rootDir[1000];

Bpb readbpb(unsigned char controller, unsigned char driveNumber, unsigned int lba) {
    Bpb bpb;
    ata_read(controller, driveNumber, lba, 1, &bpb);
    return bpb;
}

unsigned int clusterToSector(unsigned int cluster, Bpb bpb, unsigned int lba) {
    unsigned int cluster2address = lba + (bpb.numHiddenSectors*bpb.bytesPerSector + bpb.sectorsPerFat*bpb.bytesPerSector*bpb.numFats + bpb.numRootDirEntries*32); // location on disk of cluster #2 (first data cluster)
    unsigned int clusterAddress = cluster2address + (cluster-2)*bpb.sectorsPerCluster*bpb.bytesPerSector;
    return clusterAddress / 512; // assumes multiple of 512
}

void listRootDir(unsigned char controller, unsigned char driveNumber, unsigned int lba, DirEntry* entries) {
    Bpb bpb = readbpb(controller, driveNumber, lba);
    unsigned short root_dir_sectors = ((bpb.numRootDirEntries * 32) + (bpb.bytesPerSector - 1)) / bpb.bytesPerSector;
    unsigned short root_dir_start = bpb.reservedSectors + (bpb.numFats * bpb.sectorsPerFat);
    ata_read(controller, driveNumber, lba + root_dir_start, root_dir_sectors, entries);
}

// reads a file from the specified disk, with the specified name and extension, and the specified partition-start lba (given by reading the partition table)
int readFile(unsigned char controller, unsigned char driveNumber, char* name, char* extension, unsigned int lba, void* buffer) {
    listRootDir(controller, driveNumber, lba, rootDir);
    for (int i = 0; rootDir[i].name[0] != 0; i++) {
        // strings must be null-terminated
        char nameStr[9] = {0};
        memcpy(rootDir[i].name, nameStr, 8);
        char extStr[4] = {0};
        memcpy(rootDir[i].extension, extStr, 3);
        if (strcmp(name, nameStr) && strcmp(extension, extStr)) {
            Bpb bpb = readbpb(controller, driveNumber, lba);
            unsigned int readSector = clusterToSector(rootDir[i].cluster, bpb, lba);
            return ata_read(controller, driveNumber, readSector, rootDir[i].size / 512 + 1, buffer);
        }
    }
    return 1;
}