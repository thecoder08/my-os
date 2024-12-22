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
    out(base + 7, 0xec); // ATA IDENTIFY
    unsigned char status = in(base + 7);
    if (status == 0 || status == 0xff) {
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
    drives.primaryMaster = identifyDrive(PRIMARY_BASE, 0xe0);
    drives.primarySlave = identifyDrive(PRIMARY_BASE, 0xf0);
    drives.secondaryMaster = identifyDrive(SECONDARY_BASE, 0xe0);
    drives.secondarySlave = identifyDrive(SECONDARY_BASE, 0xf0);
    return drives;
}

void reset(unsigned short base) {
    // performs a software reset for all drives on bus base
    out(base + 0x206, 0x06);
    for (int i = 0; i < 100000000; i++);
    out(base + 0x206, 0x02);
}

// parameters: controller - 0 for primary, 1 for secondary. drive - 0 for master, 1 for slave. lba - position to read/write. sectors - number of sectors to read/write. buffer - pointer to buffer to read/write
int ata_write(unsigned char controller, unsigned char driveNumber, unsigned int lba, unsigned char sectors, void* buffer) {
    unsigned short base;
    unsigned char drive;
    if (controller) {
        base = SECONDARY_BASE;
    }
    else {
        base = PRIMARY_BASE;
    }
    if (driveNumber) {
        drive = 0xf0;
    }
    else {
        drive = 0xe0;
    }
    reset(base);
    // disable interrupts
    out(base + 0x206, 0x02);
    out(base + 6, drive| ((lba >> 24) & 0x0F));
    for (int i = 0; i < 100000000; i++);
    out(base + 1, 0);
    out(base + 2, sectors);
    out(base + 3, (unsigned char) lba);
    out(base + 4, (unsigned char)(lba >> 8));
    out(base + 5, (unsigned char)(lba >> 16));
    out(base + 7, 0x30);
    // loop through all sectors
    for (unsigned short sector = 0; sector < sectors; sector++) {
        int busyTest = 0;
        while (1) {
            if (busyTest > 1000) {
                print("Drive hang, resetting.\r\n");
                reset(base);
                return 1;
            }
            unsigned char status = in(base + 7);
            print("0x");
            print(itoa(status, 16));
            print("\r\n");
            if ((status & 0x80) && !(status & 0x40)) { // wait for BSY to clear and RDY to set
                busyTest++;
                continue;
            }
            if (status & 0x21) { // ensure no error
                unsigned char error = in(base + 1);
                print("Got error 0x");
                print(itoa(error, 16));
                print("\r\n");
                return 1;
            }
            if (status & 0x08) { // wait for DRQ to set
                // transfer 256 words from disk
                print("reading block\r\n");
                for (unsigned short i = 0; i < 256; i++) {
                    outs(base, ((unsigned short*)buffer)[sector * 256 + i]);
                }
                break;
            }
            busyTest++; // DRQ not set
        }
    }
    return 0;
}
int ata_read(unsigned char controller, unsigned char driveNumber, unsigned int lba, unsigned char sectors, void* buffer) {
    unsigned short base;
    unsigned char drive;
    if (controller) {
        base = SECONDARY_BASE;
    }
    else {
        base = PRIMARY_BASE;
    }
    if (driveNumber) {
        drive = 0xf0;
    }
    else {
        drive = 0xe0;
    }
    reset(base);
    // disable interrupts
    out(base + 0x206, 0x02);
    out(base + 6, drive| ((lba >> 24) & 0x0F));
    for (int i = 0; i < 100000000; i++);
    out(base + 1, 0);
    out(base + 2, sectors);
    out(base + 3, (unsigned char) lba);
    out(base + 4, (unsigned char)(lba >> 8));
    out(base + 5, (unsigned char)(lba >> 16));
    out(base + 7, 0x20);
    // loop through all sectors
    for (unsigned short sector = 0; sector < sectors; sector++) {
        int busyTest = 0;
        while (1) {
            if (busyTest > 1000) {
                print("Drive hang, resetting.\r\n");
                reset(base);
                return 1;
            }
            unsigned char status = in(base + 7);
            print("0x");
            print(itoa(status, 16));
            print("\r\n");
            if ((status & 0x80) && !(status & 0x40)) { // wait for BSY to clear and RDY to set
                busyTest++;
                continue;
            }
            if (status & 0x21) { // ensure no error
                unsigned char error = in(base + 1);
                print("Got error 0x");
                print(itoa(error, 16));
                print("\r\n");
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
            busyTest++; // DRQ not set
        }
    }
    return 0;
}

// parameters: controller - 0 for primary, 1 for secondary. drive - 0 for master, 1 for slave. lba - position to read/write. sectors - number of sectors to read/write. buffer - pointer to buffer to read/write
int ata_write48(unsigned char controller, unsigned char driveNumber, unsigned char* lba, unsigned short sectors, void* buffer) {
    unsigned short base;
    unsigned char drive;
    if (controller) {
        base = SECONDARY_BASE;
    }
    else {
        base = PRIMARY_BASE;
    }
    if (driveNumber) {
        drive = 0xf0;
    }
    else {
        drive = 0xe0;
    }
    reset(base);
    // disable interrupts
    out(base + 0x206, 0x02);

    out(base + 6, drive);
    for (int i = 0; i < 100000000; i++);
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
        int busyTest = 0;
        while (1) {
            if (busyTest > 1000) {
                print("Drive hang, resetting.\r\n");
                reset(base);
                return 1;
            }
            unsigned char status = in(base + 7);
            print("0x");
            print(itoa(status, 16));
            print("\r\n");
            if ((status & 0x80) && !(status & 0x40)) { // wait for BSY to clear and RDY to set
                busyTest++;
                continue;
            }
            if (status & 0x21) { // ensure no error
                unsigned char error = in(base + 1);
                print("Got error 0x");
                print(itoa(error, 16));
                print("\r\n");
                return 1;
            }
            if (status & 0x08) { // wait for DRQ to set
                // transfer 256 words from disk
                print("reading block\r\n");
                for (unsigned short i = 0; i < 256; i++) {
                    outs(base, ((unsigned short*)buffer)[sector * 256 + i]);
                }
                break;
            }
            busyTest++; // DRQ not set
        }
    }
    return 0;
}
int ata_read48(unsigned char controller, unsigned char driveNumber, unsigned char* lba, unsigned short sectors, void* buffer) {
    unsigned short base;
    unsigned char drive;
    if (controller) {
        base = SECONDARY_BASE;
    }
    else {
        base = PRIMARY_BASE;
    }
    if (driveNumber) {
        drive = 0xf0;
    }
    else {
        drive = 0xe0;
    }
    reset(base);
    // disable interrupts
    out(base + 0x206, 0x02);
    out(base + 6, drive);
    for (int i = 0; i < 100000000; i++);
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
            if (busyTest > 1000) {
                print("Drive hang, resetting.\r\n");
                reset(base);
                return 1;
            }
            unsigned char status = in(base + 7);
            print("0x");
            print(itoa(status, 16));
            print("\r\n");
            if ((status & 0x80) && !(status & 0x40)) { // wait for BSY to clear and RDY to set
                busyTest++;
                continue;
            }
            if (status & 0x21) { // ensure no error
                unsigned char error = in(base + 1);
                print("Got error 0x");
                print(itoa(error, 16));
                print("\r\n");
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
            busyTest++; // DRQ not set
        }
    }
    return 0;
}