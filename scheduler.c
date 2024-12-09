#include "scheduler.h"
#include "serial.h"
#include "string.h"
#include "mem.h"
#include "interrupts.h"

void* espVal;

Process processTable[100]; // TODO: Make this a linked list
int runningProcesses = 0; // total number of loaded processes
int activeProcess = 10; // index of process that is currently running. We start it at 10 so that cpu context before preemption doesn't overwrite first processes.

void* newTaskStack;
void* newTaskEntry;

void newtask();

void contextSwitch(struct interrupt_frame* frame) {
    // save esp to table
    processTable[activeProcess].stackPointer = espVal;
    // select new process
    activeProcess++;
    if (activeProcess >= runningProcesses) {
        activeProcess = 0;
    }
    // load esp from table
    espVal = processTable[activeProcess].stackPointer;
    // return
    sendEOI(0);
}

void registerProcess(void* entryPoint) {
    serialPrint("NEWTASK ");
    serialPrint(itoa(runningProcesses, 10));
    serialPrint("\r\n");
    processTable[runningProcesses].stackPointer = malloc(0x1000) + 0x1000; // we add this because the stack grows down
    newTaskEntry = entryPoint;
    newTaskStack = processTable[runningProcesses].stackPointer;
    newtask();
    processTable[runningProcesses].stackPointer -= 44; // newtask pushes a bunch of stuff to the stack
    runningProcesses++;
}