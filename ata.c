#include "ata.h"
#include "io.h"
#include "terminal.h"
#include "string.h"

#define PRIMARY_BASE 0x1f0
#define SECONDARY_BASE 0x170

// returns a struct with a member for each drive. 0: drive is absent. 1: drive is ATA, 2: drive is ATAPI.
int identifyDrive(unsigned short base, unsigned char drive) {
    // disable interrupts
    out(base + 0x206, 0x02);

    out(base + 6, drive);
    out(base + 2, 0);
    out(base + 3, 0);
    out(base + 4, 0);
    out(base + 5, 0);
    out(base + 7, 0xec);
    if (in(base + 7) == 0) {
        return 0;
    }
    // wait for BSY to clear
    while (in(base + 7) & 0x80);
    // wait for error or drq
    while(1) {
        unsigned char status = in(base + 7);
        if (status & 0x08) {
            for (int i = 0; i < 256; i++) {
                ins(base);
            }
            return 1;
        }
        if (status & 0x01) {
            return 2;
        }
    }
}

Drives detect_drives() {
    Drives drives = {0};
    drives.primaryMaster = identifyDrive(PRIMARY_BASE, 0xa0);
    drives.primarySlave = identifyDrive(PRIMARY_BASE, 0xb0);
    drives.secondaryMaster = identifyDrive(SECONDARY_BASE, 0xa0);
    drives.secondarySlave = identifyDrive(SECONDARY_BASE, 0xb0);
    return drives;
}

void reset(unsigned short base) {
    // performs a software reset for all drives on bus base
    out(base + 0x206, 0x06);
    for (int i = 0; i < 100000000; i++);
    out(base + 0x206, 0x02);
}

// parameters: controller - 0 for primary, 1 for secondary. drive - 0 for master, 1 for slave. lba - position to read/write. sectors - number of sectors to read/write. buffer - pointer to buffer to read/write
int ata_write(unsigned char controller, unsigned char driveNumber, unsigned char* lba, unsigned short sectors, void* buffer) {
    unsigned short base;
    unsigned char drive;
    if (controller) {
        base = SECONDARY_BASE;
    }
    else {
        base = PRIMARY_BASE;
    }
    if (driveNumber) {
        drive = 0x50;
    }
    else {
        drive = 0x40;
    }
    reset(base);
    // disable interrupts
    out(base + 0x206, 0x02);

    out(base + 6, drive);
    out(base + 2, sectors >> 8);
    out(base + 3, lba[3]);
    out(base + 4, lba[4]);
    out(base + 5, lba[5]);
    out(base + 2, sectors);
    out(base + 3, lba[0]);
    out(base + 4, lba[1]);
    out(base + 5, lba[2]);
    out(base + 7, 0x34);
    // loop through all sectors
    for (unsigned short sector = 0; sector < sectors; sector++) {
        // wait for drq or error
        while (1) {
            unsigned char status = in(base + 7);
            if (status & 0x01) {
                return 1;
            }
            if (status & 0x08) {
                for (unsigned short i = 0; i < 256; i++) {
                    outs(base, ((unsigned short*)buffer)[sector * 256 + i]);
                }
                break;
            }
        }
    }
    return 0;
}

int ata_read(unsigned char controller, unsigned char driveNumber, unsigned char* lba, unsigned short sectors, void* buffer) {
    unsigned short base;
    unsigned char drive;
    if (controller) {
        base = SECONDARY_BASE;
    }
    else {
        base = PRIMARY_BASE;
    }
    if (driveNumber) {
        drive = 0x50;
    }
    else {
        drive = 0x40;
    }
    reset(base);
    // disable interrupts
    out(base + 0x206, 0x02);

    out(base + 6, drive);
    out(base + 2, sectors >> 8);
    out(base + 3, lba[3]);
    out(base + 4, lba[4]);
    out(base + 5, lba[5]);
    out(base + 2, sectors);
    out(base + 3, lba[0]);
    out(base + 4, lba[1]);
    out(base + 5, lba[2]);
    out(base + 7, 0x24);
    // loop through all sectors
    for (unsigned short sector = 0; sector < sectors; sector++) {
        int busyTest = 0;
        while (1) {
            if (busyTest > 20) {
                print("Drive hang, resetting.\r\n");
                reset(base);
                return 1;
            }
            unsigned char status = in(base + 7);
            print("0x");
            print(itoa(status, 16));
            print("\r\n");
            if (status & 0x80) { // wait for BSY to clear
                busyTest++;
                continue;
            }
            if (status & 0x01) { // ensure no error
                return 1;
            }
            if (status & 0x08) { // wait for DRQ to set
                // transfer 256 words from disk
                print("reading block\r\n");
                for (unsigned short i = 0; i < 256; i++) {
                    ((unsigned short*)buffer)[sector * 256 + i] = ins(base);
                }
                break;
            }
        }
    }
    return 0;
}