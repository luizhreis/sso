#include <stdio.h>
#include <stdlib.h>

#include "processManager.h"
#include "fifoQueues.h"
#include "process.h"

void initiateProcessManager(struct ProcessManager *manager){
    manager->highPriorityQueue = createQueue();
    manager->lowPriorityQueue = createQueue();
    manager->ioQueue = createQueue();
}

void generateProcess(struct ProcessManager *manager, int ppid, int priority, int burstTime){
    struct Process *process = createProcess(manager->nextPid, ppid, priority, burstTime);
    manager->processList[manager->nextPid] = process;
    enQueue(manager->highPriorityQueue, process->pid);
    manager->nextPid = generateNextPid(manager->nextPid);
}

int main(){
    const unsigned int maxProcess = 1024;
    const unsigned int timeSlice = 4;
    const unsigned int executionTimeLimit = 10000;
    int unsigned executionTime = 0;
    int unsigned partialTime = 0;
    // struct Process *(processList[maxProcess]);
    struct Process *tmpProc;
    struct Process *teste;
    struct Node *pidRunning = NULL;
    struct Node *ioRunning = NULL;
    struct Process *processRunning;
    struct ProcessManager *manager = createProcessManager();

    initiateProcessManager(manager);

    generateProcess(manager, 0, 0, 10);
    show(manager->highPriorityQueue);
    show(manager->lowPriorityQueue);
    generateProcess(manager, 0, 0, 20);
    show(manager->highPriorityQueue);
    show(manager->lowPriorityQueue);

    teste = manager->processList[0];
    printf("TESTE: %d\n", teste->pid);
    teste = manager->processList[1];
    printf("TESTE: %d\n", teste->pid);

    while(executionTime < executionTimeLimit){
        if(!pidRunning){
            if(!isEmpty(manager->highPriorityQueue)){
                pidRunning = deQueue(manager->highPriorityQueue);
                processRunning = manager->processList[pidRunning->data];
                processRunning->state = 1;
            }
            else if(!isEmpty(manager->lowPriorityQueue)){
                pidRunning = deQueue(manager->lowPriorityQueue);
                processRunning = manager->processList[pidRunning->data];
                processRunning->state = 1;
            }
            
        }
        if(processRunning != NULL){
            processRunning->burstTime -= 1;
            // printf("BURST TIME: %d\n", processRunning->burstTime);
            partialTime++;
        }
        if(processRunning->burstTime == 0){
            free(processRunning);
            manager->processList[pidRunning->data] = NULL;
            processRunning = NULL;
            pidRunning = NULL;
            partialTime = 0;
        }
        else if(processRunning->burstTime > 0 && timeSlice == partialTime ){
            enQueue(manager->lowPriorityQueue, processRunning->pid);
            processRunning->state = 0;
            pidRunning = NULL;
            partialTime = 0;
        }
        show(manager->highPriorityQueue);
        show(manager->lowPriorityQueue);
        show(manager->ioQueue);
        executionTime++;
        getchar();
    }

    printf("\n\n\n");


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

    // printf("PID: %d, BURST TIME: %f\n", proc1->pid, proc1->burstTime);
    // printf("PID: %d, BURST TIME: %f\n", proc2->pid, proc2->burstTime);

    struct Node *n = deQueue(manager->highPriorityQueue);
    if(n != NULL)
        printf("Dequeued item is %d\n", n->data);
    return 0;
}