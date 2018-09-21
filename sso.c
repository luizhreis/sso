#include <stdio.h>
#include <stdlib.h>

#include "processManager.h"
#include "fifoQueues.h"
#include "process.h"
#include "parseInputFile.h"

void initiateProcessManager(struct ProcessManager *manager){
    manager->highPriorityQueue = createQueue();
    manager->lowPriorityQueue = createQueue();
    manager->ioQueue = createQueue();
}

void generateProcess(struct ProcessManager *manager, int ppid, int priority, int arrivalTime){
    struct Process *process = createProcess(manager->nextPid, ppid, priority, arrivalTime);
    printf("PROCESS CREATED: pid = %d, priority = %d, arrival time = %d, execution time = %d\n", process->pid, process->priority, process->arrivalTime, process->burstTime);
    manager->processList[manager->nextPid] = process;
    enQueue(manager->highPriorityQueue, process->pid);
    manager->nextPid = generateNextPid(manager->nextPid);
}

int main(){
    const unsigned int maxProcess = 1024;
    const unsigned int timeSlice = 4;
    const unsigned int simulationTimeLimit = 10000;
    unsigned int simulationTime = 0;
    unsigned int partialTime = 0;
    // struct Process *(processList[maxProcess]);
    struct Process *tmpProc;
    struct Process *teste;
    struct Node *pidRunning = NULL;
    struct Node *ioRunning = NULL;
    struct Node *newProcessCreation = NULL;
    struct Process *processRunning;
    struct ProcessManager *manager = createProcessManager();
    struct Queue *processCreation = createQueue();

    parseProcessFile("process.txt", processCreation);
    show(processCreation);

    initiateProcessManager(manager);

    // generateProcess(manager, 0, 0, 10);
    // show(manager->highPriorityQueue);
    // show(manager->lowPriorityQueue);
    // generateProcess(manager, 0, 0, 20);
    show(manager->highPriorityQueue);
    show(manager->lowPriorityQueue);

    // teste = manager->processList[0];
    // printf("TESTE: %d\n", teste->pid);
    // teste = manager->processList[1];
    // printf("TESTE: %d\n", teste->pid);

    newProcessCreation = deQueue(processCreation);
    // printf("NOVO PROC: time creation = %d, priority = %d\n", newProcessCreation->data, newProcessCreation->priority);
    while(simulationTime < simulationTimeLimit){
        printf("SIMULATION TIME = %d\n", simulationTime);
        while(newProcessCreation != NULL && newProcessCreation->data == simulationTime){
            // printf("NOVO PROC: time creation = %d, priority = %d\n", newProcessCreation->data, newProcessCreation->priority);
            generateProcess(manager, 0, newProcessCreation->priority, simulationTime);
            free(newProcessCreation);
            newProcessCreation = deQueue(processCreation);
        }
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
            printf("PROCESS RUNNING: pid = %d, priority = %d, execution time = %d\n", processRunning->pid, processRunning->priority, processRunning->burstTime);
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
            if(processRunning->priority == 0){
                enQueue(manager->highPriorityQueue, processRunning->pid);
            }
            else{
                enQueue(manager->lowPriorityQueue, processRunning->pid);
            }
            processRunning->state = 0;
            pidRunning = NULL;
            partialTime = 0;
        }
        show(manager->highPriorityQueue);
        show(manager->lowPriorityQueue);
        show(manager->ioQueue);
        simulationTime++;
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