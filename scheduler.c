#include "scheduler.h"
#include "serial.h"
#include "string.h"
#include "mem.h"

void* oldEsp;
void* newEsp;

Process processTable[100];
int runningProcesses = 0; // total number of loaded processes
int activeProcess = 10; // index of process that is currently running

void* newTaskStack;
void* newTaskEntry;

void ctxsw();
void newtask();

__attribute__((interrupt)) void contextSwitch(struct interrupt_frame* frame) {
    // save esp to table
    processTable[activeProcess].stackPointer = oldEsp;
    // select new process
    activeProcess++;
    if (activeProcess >= runningProcesses) {
        activeProcess = 0;
    }
    serialPrint("CTXSW PID ");
    serialPrint(itoa(activeProcess, 10));
    serialPrint("\r\n");
    // load esp from table
    newEsp = processTable[activeProcess].stackPointer;
    // return
}

void registerProcess(void* entryPoint) {
    serialPrint("NEWTASK ");
    serialPrint(itoa(runningProcesses, 10));
    serialPrint("\r\n");
    processTable[runningProcesses].entryPointer = entryPoint;
    processTable[runningProcesses].stackPointer = malloc(0x1000) + 0x1000; // we add this because the stack grows down
    newTaskEntry = entryPoint;
    newTaskStack = processTable[runningProcesses].stackPointer;
    newtask();
    runningProcesses++;
}