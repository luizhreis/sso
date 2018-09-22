#include <stdio.h>
#include <stdlib.h>

#include "processManager.h"
#include "fifoQueues.h"
#include "process.h"
#include "parseInputFile.h"

#define C_RESET       "\033[0m"       // Clear
#define C_BOLD        "\033[1m"       // Bold
#define C_BLACK       "\033[30m"      // Black
#define C_RED         "\033[31m"      // Red
#define C_GREEN       "\033[32m"      // Green
#define C_YELLOW      "\033[33m"      // Yellow
#define C_BLUE        "\033[34m"      // Blue
#define C_MAGENTA     "\033[35m"      // Magenta
#define C_CYAN        "\033[36m"      // Cyan
#define C_WHITE       "\033[37m"      // White

void initiateProcessManager(struct ProcessManager *manager){
    manager->highPriorityQueue = createQueue();
    manager->lowPriorityQueue = createQueue();
    manager->ioQueue = createQueue();
}

void generateProcess(struct ProcessManager *manager, int ppid, int priority, int arrivalTime){
    struct Process *process = createProcess(manager->nextPid, ppid, priority, arrivalTime);
    fprintf(stdout, C_GREEN "%s" C_RESET ": pid = %d, priority = %d, execution time = %d\n", "Process Created", process->pid, process->priority, process->burstTime);
    // printf("PROCESS CREATED: pid = %d, priority = %d, arrival time = %d, execution time = %d\n", process->pid, process->priority, process->arrivalTime, process->burstTime);
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
    // show(processCreation);

    initiateProcessManager(manager);

    newProcessCreation = deQueue(processCreation);
    while(simulationTime < simulationTimeLimit){
        system("clear");
        fprintf(stdout, C_YELLOW "%s" C_RESET ": %d\n", "Simulation Time", simulationTime);
        while(newProcessCreation != NULL && newProcessCreation->data == simulationTime){
            generateProcess(manager, 0, newProcessCreation->priority, simulationTime);
            free(newProcessCreation);
            newProcessCreation = deQueue(processCreation);
            if(!newProcessCreation)
                newProcessCreation = newNode(simulationTimeLimit + 1);
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
            fprintf(stdout, C_YELLOW "%s" C_RESET ": pid = %d, priority = %d, execution time = %d\n", "Process Running", processRunning->pid, processRunning->priority, processRunning->burstTime);
            // printf("PROCESS RUNNING: pid = %d, priority = %d, execution time = %d\n", processRunning->pid, processRunning->priority, processRunning->burstTime);
            partialTime++;
        }
        if(processRunning->burstTime == 0){
            fprintf(stdout, C_RED "%s" C_RESET ": pid = %d, priority = %d\n", "Process Terminated", processRunning->pid, processRunning->priority);
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
        fprintf(stdout, C_BOLD "\n%s" C_RESET, "High Priority Queue:\t");
        show(manager->highPriorityQueue);
        fprintf(stdout, C_BOLD "%s" C_RESET, "Low Priority Queue:\t");
        show(manager->lowPriorityQueue);
        fprintf(stdout, C_BOLD "%s" C_RESET, "I/O Queue:\t\t");
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

    struct Node *n = deQueue(manager->highPriorityQueue);
    if(n != NULL)
        printf("Dequeued item is %d\n", n->data);
    return 0;
}