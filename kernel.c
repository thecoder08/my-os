#include "gdt.h"
#include "interrupts.h"
#include "mem.h"
#include "keyboard.h"
#include "graphics.h"
#include "gui.h"
#include "terminal.h"
#include "string.h"
#include "vga.h"
#include "timer.h"
#include "parttable.h"
#include "fat16.h"
#include "ata.h"
#include "syscall.h"
#include "scheduler.h"
#include "pong.h"

int textMode = 0;

void shellEntry() { // this has to be made into its own function, so that it can be preempted
      print("Welcome to My OS! Use the \"help\" command for help. This is the basic shell. To get a more advanced shell, run \"loadshell\".\r\n");
      enableCursor(14, 15);
      char input[0x100];
      while(1) {
            print("> ");
            scan(input);
            if (strcmp(input, "")) {}
            else if (strcmp(input, "help")) {
                  print("help: Print this message.\r\n");
                  print("clear: Clear the screen.\r\n");
                  print("cttyVGA: Change the TTY to VGA.\r\n");
                  print("cttySERIAL: Change the TTY to the serial port.\r\n");
                  print("halt: Stop the OS.\r\n");
                  print("graphics: Try drawing to the screen.\r\n");
                  print("gui: Launch the GUI.\r\n");
                  print("pong: Try the pong demo. Must be launched after gui.\r\n");
                  print("ATA: enumerate ATA drives.\r\n");
                  print("loadshell: Tries loading and running SHELL.BIN from the root directory of FAT16 formatted HDD.\r\n");

            }
            else if (strcmp(input, "clear")) {
                  clearText(' ', 0x07);
            }
            else if (strcmp(input, "cttyVGA")) {
                  ctty(1);
            }
            else if (strcmp(input, "cttySERIAL")) {
                  ctty(0);
            }
            else if (strcmp(input, "halt")) {
                  asm("cli");
                  while(1) {asm("hlt");}
            }
            else if (strcmp(input, "graphics")) {
                  circle(100, 100, 10, 0xffff0000);
            }
            else if (strcmp(input, "gui")) {
                  registerProcess(initGui);
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
            else if (strcmp(input, "loadshell")) {
                  PartTableEntry entries[4];
                  readPartitionTable(0, 0, entries); // read and print partition table
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
                  if (entries[0].type == 4) { // if first partition is fat16-formatted, try loading and running shell.bin
                        int readStatus = readFile(0, 0, "SHELL   ", "BIN", entries[0].lba, (void*) 0x20000);
                        if (readStatus == 0) {
                              print("Loaded shell sucessfully.\r\n");
                              //asm("call $0x08,$0x20000");
                              registerProcess((void*) 0x20000); // woohoo, I can do this now!
                              while(1);
                        }
                        else {
                              print("Failed to load shell.\r\n");
                        }
                  }
            }
            else if (strcmp(input, "pong")) {
                  registerProcess(initPong);
            }
            else {
                  print("I'm not sure what \"");
                  print(input);
                  print("\" means.\r\n");
            }
      }
}

Framebuffer readFramebufferInfo(void* mbInfo) {
      Framebuffer fb;
      fb.data = 0; // if we can't find a framebuffer, data is 0
      unsigned int flags = *(unsigned int*)mbInfo;
      if (flags & (1 << 12)) { // if fb section exists
            unsigned int type = *(unsigned int*)(mbInfo + 109);
            if (type == 2) { // if fb type is not text mode
                  return fb;
            }
            fb.data = *(int**)(mbInfo + 88);
            fb.width = *(int*)(mbInfo + 100);
            fb.height = *(int*)(mbInfo + 104);
      }
      return fb;
}

void kmain() {
      int magic;
      void* mbInfo;
      asm("" : "=a" (magic), "=b" (mbInfo):);
      Framebuffer fb = readFramebufferInfo(mbInfo);
      if (fb.data) { // using graphics mode
            textMode = 0;
            setFb(fb);
            circle(500, 500, 100, 0xff00ff00);
      }
      else {
            textMode = 1;
      }
      clearText(' ', 0x07);
      if (textMode) {
            print("Using VGA text mode.\r\n");
      }
      else {
            print("Using framebuffer console.\r\n");
      }
      initializeGdt();
      initializeIdt();
      initKeyboard();
      initSyscall();
      print("magic number: 0x");
      print(itoa(magic, 16));
      print("\r\n");
      initScheduler();
      init_timer(1); // enable preemption
      print("Preemption enabled.\r\n");
      shellEntry(); // First process, PID 0, is already here. It's just whatever was running before first preemption.
}