#include "parttable.h"
#include "ata.h"

void readPartitionTable(unsigned char controller, unsigned char driveNumber, PartTableEntry* entries) {
    unsigned char mbr[512];
    ata_read(controller, driveNumber, 0, 1, mbr);
    for (int i = 0; i < 4; i++) {
        entries[i].status = mbr[0x1be + 16*i];
        entries[i].chsStart[0] = mbr[0x1be + 16*i + 1];
        entries[i].chsStart[0] = mbr[0x1be + 16*i + 2];
        entries[i].chsStart[0] = mbr[0x1be + 16*i + 3];
        entries[i].type = mbr[0x1be + 16*i + 4];
        entries[i].chsStart[0] = mbr[0x1be + 16*i + 5];
        entries[i].chsStart[0] = mbr[0x1be + 16*i + 6];
        entries[i].chsStart[0] = mbr[0x1be + 16*i + 7];
        entries[i].lba = mbr[0x1be + 16*i + 8] | (mbr[0x1be + 16*i + 9] << 8) | (mbr[0x1be + 16*i + 10] << 16) | (mbr[0x1be + 16*i + 11] << 24);
        entries[i].length = mbr[0x1be + 16*i + 12] | (mbr[0x1be + 16*i + 13] << 8) | (mbr[0x1be + 16*i + 14] << 16) | (mbr[0x1be + 16*i + 15] << 24);
    }
}