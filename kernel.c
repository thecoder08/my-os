#include "gdt.h"
#include "keyboard.h"
#include "mouse.h"
#include "terminal.h"
#include "mem.h"
#include "string.h"
#include "vga.h"
#include "interrupts.h"
#include "timer.h"
#include "parttable.h"
#include "fat16.h"
#include "ata.h"

char input[0x100];

void mouse(unsigned char flags, unsigned char x, unsigned char y) {
      print("Flags: ");
      print(itoa(flags, 2));
      print(" x: ");
      print(itoa(x, 10));
      print(" y: ");
      print(itoa(y, 10));
      print("\r\n");
}

void kbdScan() {
      if (strcmp(input, "")) {}
      else if (strcmp(input, "help")) {
            print("help: Print this message.\r\n");
            print("clear: Clear the screen.\r\n");
            print("cttyVGA: Change the TTY to VGA.\r\n");
            print("cttySERIAL: Change the TTY to the serial port.\r\n");
            print("halt: Stop the OS.\r\n");
            print("graphics: Try changing the graphics mode and drawing to the screen.\r\n");
      }
      else if (strcmp(input, "clear")) {
            clear(' ', 0x07);
      }
      else if (strcmp(input, "cttyVGA")) {
            ctty(1);
      }
      else if (strcmp(input, "cttySERIAL")) {
            ctty(0);
      }
      else if (strcmp(input, "halt")) {
            while(1);
      }
      else if (strcmp(input, "graphics")) {
            setVideoMode(1);
            memset((char*) 0x000a0000, 4, 50);
      }
      else if (strcmp(input, "mouse")) {
            initMouse(mouse);
      }
      else if (strcmp(input, "ATA")) {
            print("Running ATA drive enumeration.\r\n");
            Drives drives = detect_drives();
            print("Primary master: ");
            if (drives.primaryMaster == 0) {
                  print("absent.\r\n");
            }
            if (drives.primaryMaster == 1) {
                  print("ATA.\r\n");
            }
            if (drives.primaryMaster == 2) {
                  print("ATAPI.\r\n");
            }
            print("Primary slave: ");
            if (drives.primarySlave == 0) {
                  print("absent.\r\n");
            }
            if (drives.primarySlave == 1) {
                  print("ATA.\r\n");
            }
            if (drives.primarySlave == 2) {
                  print("ATAPI.\r\n");
            }
            print("Secondary master: ");
            if (drives.secondaryMaster == 0) {
                  print("absent.\r\n");
            }
            if (drives.secondaryMaster == 1) {
                  print("ATA.\r\n");
            }
            if (drives.secondaryMaster == 2) {
                  print("ATAPI.\r\n");
            }
            print("Secondary slave: ");
            if (drives.secondarySlave == 0) {
                  print("absent.\r\n");
            }
            if (drives.secondarySlave == 1) {
                  print("ATA.\r\n");
            }
            if (drives.secondarySlave == 2) {
                  print("ATAPI.\r\n");
            }
      }
      else if (strcmp(input, "disktest")) {
            /*unsigned char lba[6] = {0};
            int readStatus = ata_read(0, 1, lba, 17, (unsigned char*) 0x10000);
            print("Read returned ");
            print(itoa(readStatus, 10));
            print(".\r\n");
            print("done\r\n");
            asm("jmp $0x08,$0x10000");
            while(1);*/

            PartTableEntry entries[4];
            readPartitionTable(0, 0, entries);
            for (int i = 0; i < 4; i++) {
                  print("Partition ");
                  print(itoa(i + 1, 10));
                  if (entries[i].type == 0) {
                        print(": ABSENT\r\n");
                  }
                  else {
                        print(": type 0x");
                        print(itoa(entries[i].type, 16));
                        print(" start 0x");
                        print(itoa(entries[i].lba, 16));
                        print(" size 0x");
                        print(itoa(entries[i].length, 16));
                        print("\r\n");
                  }
            }
            if (entries[0].type == 4) { // if first partition is fat16-formatted, print BPB/EBPB
                  static RootDirEntry rootdir[1000];
                  listRootDir(0, 0, entries[0].lba, rootdir);
                  for (int i = 0; rootdir[i].name[0] != 0; i++) {
                        if (rootdir[i].attributes == 0x0f) {
                              continue;
                        }
                        print("Entry ");
                        print(itoa(i + 1, 10));
                        print(": ");
                        char name[13] = {0};
                        memcpy(rootdir[i].name, name, 8);
                        name[8] = '.';
                        memcpy(rootdir[i].extension, name + 9, 3);
                        print(name);
                        print(" cluster ");
                        print(itoa(rootdir[i].cluster, 16));
                        print(" size ");
                        print(itoa(rootdir[i].size, 16));
                        print("\r\n");
                  }
            }
      }
      else if (strcmp(input, "div0")) {
            int x = 10 / 0;
      }
      else if (strcmp(input, "timer")) {
            init_timer(50);
      }
      else {
            print("I'm not sure what \"");
            print(input);
            print("\" means.\r\n");
      }
      print("> ");
}

void kmain() {
      clear(' ', 0x07);
      initializeGdt();
      initializeIdt();
      init_timer(50);
      initKeyboard(input, kbdScan);
      print("Welcome to My OS! Use the \"help\" command for help. Loading programs from disk isn't yet supported so only builtin commands will work.\r\n");
      enableCursor(14, 15);
      print("> ");
      while(1);
}