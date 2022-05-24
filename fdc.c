#include "io.h"
#define SECTORS_PER_TRACK 18

void lba_to_chs(int lba, int *head, int *track, int *sector) {
    (*head) = (lba % (SECTORS_PER_TRACK * 2)) / SECTORS_PER_TRACK;
	(*track) = (lba / (SECTORS_PER_TRACK * 2));
	(*sector) = (lba % SECTORS_PER_TRACK + 1);
}

void read_blocks(int blocks, int drive, int lba, char* buffer) {
    // set dor
    out(0x3f2, (0x10 << drive) | drive);
    // get chs
    int head;
    int track;
    int sector;
    lba_to_chs(lba, &head, &track, &sector);
    // wait until the drive is ready
    while((in(0x3f4) & 0x80) != 0x80);
    // send command and parameters
    out(0x3f5, 0x80 | 0x40 | 0x6);
    out(0x3f5, (head << 2) | drive);
    out(0x3f5, track);
    out(0x3f5, head);
    out(0x3f5, sector);
    out(0x3f5, 2);
    out(0x3f5, SECTORS_PER_TRACK);
    out(0x3f5, 0x1b);
    out(0x3f5, 0xff);
    int i = 0;
    // while we haven't read all our data
    while (i < blocks * 512) {
        // if we can read data
        if ((in(0x3f4) & 0x40) == 0x40) {
            // read it into the buffer
            buffer[i] = in(0x3f5);
            i++;
        }
    }
}

void write_blocks(int blocks, int drive, int lba, char* buffer) {
    // set dor
    out(0x3f2, (0x10 << drive) | drive);
    // get chs
    int head;
    int track;
    int sector;
    lba_to_chs(lba, &head, &track, &sector);
    // wait until the drive is ready
    while((in(0x3f4) & 0x80) != 0x80);
    // send command and parameters
    out(0x3f5, 0x80 | 0x40 | 0x5);
    out(0x3f5, (head << 2) | drive);
    out(0x3f5, track);
    out(0x3f5, head);
    out(0x3f5, sector);
    out(0x3f5, 2);
    out(0x3f5, SECTORS_PER_TRACK);
    out(0x3f5, 0x1b);
    out(0x3f5, 0xff);
    int i = 0;
    // while we haven't written all our data
    while (i < blocks * 512) {
        // if we can write data
        if ((in(0x3f4) & 0x80) == 0x80) {
            // write it to the disk
            out(0x3f5, buffer[i]);
            i++;
        }
    }
}