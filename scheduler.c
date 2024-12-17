#include "scheduler.h"
#include "serial.h"
#include "string.h"
#include "mem.h"
#include "interrupts.h"

void* espVal;

Process* activeProcess;
int newProcessId = 0;

void* newTaskStack;
void* newTaskEntry;

void newtask();

void contextSwitch(struct interrupt_frame* frame) {
    sendEOI(0);
    // save esp to table
    activeProcess->stackPointer = espVal;
    // select new process
    activeProcess = activeProcess->next;
    // load esp from table
    espVal = activeProcess->stackPointer;
    // return
}

int registerProcess(void* entryPoint) {
    newProcessId++;
    serialPrint("NEWTASK ");
    serialPrint(itoa(newProcessId, 10));
    serialPrint("\r\n");
    Process* newProcess = malloc(sizeof(Process));
    newProcess->pid = newProcessId;
    newProcess->next = activeProcess->next;
    activeProcess->next = newProcess;
    newProcess->stackPointer = malloc(0x1000) + 0x1000; // we add this because the stack grows down
    newTaskEntry = entryPoint;
    newTaskStack = newProcess->stackPointer;
    newtask();
    newProcess->stackPointer -= 44; // newtask pushes a bunch of stuff to the stack
    return newProcess->pid;
}

void initScheduler() {
    activeProcess = malloc(sizeof(Process));
    activeProcess->next = activeProcess;
    activeProcess->stackPointer = 0;
    activeProcess->pid = 0;
}

int deleteProcess(int killPid) { // 0 if process deleted, 1, if not found
    serialPrint("Deleting ");
    serialPrint(itoa(killPid, 10));
    serialPrint("...\r\n");
    Process* p = activeProcess;
    do {
        if (p->next->pid == killPid) {
            Process* toDelete = p->next;
            p->next = toDelete->next;
            // uncomment when we have free()
            //free(toDelete->stackPointer); // this won't work, we need to track top of stack
            //free(toDelete);
            serialPrint("Deleted ");
            serialPrint(itoa(killPid, 10));
            serialPrint("\r\n");
            return 0;
        }
        p = p->next;
    } while (p != activeProcess);
    return 1;
}