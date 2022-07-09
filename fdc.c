#include "io.h"
#include "serial.h"
#include "string.h"
#define SECTORS_PER_TRACK 18

void lba_to_chs(int lba, int *head, int *track, int *sector) {
    (*head) = (lba % (SECTORS_PER_TRACK * 2)) / SECTORS_PER_TRACK;
	(*track) = (lba / (SECTORS_PER_TRACK * 2));
	(*sector) = (lba % SECTORS_PER_TRACK + 1);
}

// send command and params to the floppy controller, exchange data via PIO if needed (either in or out), and get results.
void send_command(char command, char* params, char* data, int inorout, char* results) {
    // return if not ready 
    if ((in(0x3f4) & 0xc0) != 0x80) {
        serialPrint("NOT READY FOR COMMAND!!\r\n");
        return;
    }
    // send command byte
    out(0x3f5, command);
    serialPrint("SENT COMMAND\r\n");
    // send parameter bytes
    for (int i = 0; in(0x0f4) & 0x40; i++)
    {
        serialPrint(itoa(params[i], 16));
        serialPrint("\r\n");
        while(in(0x3f4) & 0x80);
        out(0x3f5, params[i]);
    }
    serialPrint("SENT PARAMETERS\r\n");
    // if the command has an execution phase
    if (in(0x3f4) & 0x20) {
        serialPrint("WE DO HAVE AN EXECUTION PHASE\r\n");
        // while we still have stuff to do
        while(in(0x3f4) & 0x20) {
            // wait for 
            while(in(0x3f4) & 0x80);
            for (int i = 0; in(0x3f4) & 0xa0; i++)
            {
                if (inorout) {
                    data[i] = in(0x3f5);
                }
                else {
                    out(0x3f5, data[i]);
                }
            }
            
        }
    }
    // wait for result phase
    while((in(0x3f4) & 0xc0) != 0x80);
    // read result bytes
    for (int i = 0; in(0x0f4) & 0x50; i++) {
        results[i] = in(0x3f5);
        while(in(0x3f4) & 0x80);
    }
}

void read_blocks(int blocks, int drive, int lba, char* buffer) {
    // turn on motor and select drive
    out(0x3f2, (0x10 << drive) | drive);
    // get chs
    int head;
    int track;
    int sector;
    lba_to_chs(lba, &head, &track, &sector);
    // send read command and parameters
    char params[]  = {(head << 2) | drive, track, head, sector, 2, SECTORS_PER_TRACK, 0x1b, 0xff};
    send_command(0x80 | 0x40 | 0x06, params, buffer, 1, (char*) 0);
    // turn off motor
    out(0x3f2, 0);
}

void write_blocks(int blocks, int drive, int lba, char* buffer) {
    // turn on motor and select drive
    out(0x3f2, (0x10 << drive) | drive);
    // get chs
    int head;
    int track;
    int sector;
    lba_to_chs(lba, &head, &track, &sector);
    // wait until the drive is ready
    while((in(0x3f4) & 0x10) != 0x10);
    // send recalibrate command and parameters
    out(0x3f5, 0x7);
    out(0x3f5, drive);
    // wait for recalibrate to complete
    while((in(0x3f4) & (1 << drive)) != (1 << drive));
    // sense interrupt
    out(0x3f5, 0x8);
    // wait until the drive is ready
    while((in(0x3f4) & 0x10) != 0x10);
    // send write command and parameters
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
    while (i < (blocks * 512)) {
        // if we can write data
        if ((in(0x3f4) & 0x80) == 0x80) {
            // write it to the disk
            out(0x3f5, buffer[i]);
            i++;
        }
    }
    // turn off motor
    out(0x3f2, 0);
}