#include <stdio.h>
#include <stdlib.h>

#include "processManager.h"
#include "fifoQueues.h"
#include "process.h"

const int maxProcess = 1024;

void initiateProcessManager(struct ProcessManager *manager){
    manager->highPriorityQueue = createQueue();
    manager->lowPriorityQueue = createQueue();
    manager->ioQueue = createQueue();
}

void generateProcess(struct ProcessManager *manager, int ppid, int priority, int quantum){
    struct Process *process = createProcess(manager->nextPid, ppid, priority, quantum);
    manager->processList[manager->nextPid] = process;
    enQueue(manager->highPriorityQueue, process->pid);
    manager->nextPid = generateNextPid(manager->nextPid);
}

int main(){
    // struct Process *(processList[maxProcess]);
    struct Process *tmpProc;
    struct Process *teste;
    struct ProcessManager *manager = createProcessManager();

    initiateProcessManager(manager);
    generateProcess(manager, 0, 0, 10);
    generateProcess(manager, 0, 0, 20);

    // struct Process *proc1 = generateProcess(manager->nextPid, 0, 0, 10);
    // printf("PID ATUAL: %d", manager->nextPid);
    // tmpProc = createProcess(manager->nextPid, 0, 0, 10);
    // processList[manager->nextPid] = tmpProc;

    teste = manager->processList[0];
    printf("TESTE: %d\n", teste->pid);

    // manager->nextPid = generateNextPid(manager->nextPid);
    // printf("PID ATUAL: %d", manager->nextPid);
    // enQueue(manager->highPriorityQueue, tmpProc->pid);
    // tmpProc = createProcess(manager->nextPid, 0, 0, 10);
    // processList[manager->nextPid] = tmpProc;
    // manager->nextPid = generateNextPid(manager->nextPid);
    // enQueue(manager->highPriorityQueue, tmpProc->pid);

    teste = manager->processList[1];
    printf("TESTE: %d\n", teste->pid);

    printf("lista vazia: %d\n", isEmpty(manager->highPriorityQueue));
    enQueue(manager->highPriorityQueue, 10);
    printf("lista vazia: %d\n", isEmpty(manager->highPriorityQueue));
    show(manager->highPriorityQueue);
    enQueue(manager->highPriorityQueue, 20);
    printf("lista vazia: %d\n", isEmpty(manager->highPriorityQueue));
    show(manager->highPriorityQueue);
    deQueue(manager->highPriorityQueue);
    show(manager->highPriorityQueue);
    deQueue(manager->highPriorityQueue);
    show(manager->highPriorityQueue);
    enQueue(manager->highPriorityQueue, 30);
    show(manager->highPriorityQueue);
    enQueue(manager->highPriorityQueue, 40);
    show(manager->highPriorityQueue);
    enQueue(manager->highPriorityQueue, 50);
    show(manager->highPriorityQueue);

    // printf("PID: %d, QUANTUM: %f\n", proc1->pid, proc1->quantum);
    // printf("PID: %d, QUANTUM: %f\n", proc2->pid, proc2->quantum);

    struct Node *n = deQueue(manager->highPriorityQueue);
    if(n != NULL)
        printf("Dequeued item is %d\n", n->data);
    return 0;
}