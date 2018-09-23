#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

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

static struct option long_options[] = {
    {"time_slice",      required_argument, NULL, 't'},
    {"process_limit",   required_argument, NULL, 'l'},
    {"input_file",      required_argument, NULL, 'i'},
    {"output_path",     required_argument, NULL, 'o'},
    {NULL, 0, NULL, 0}
};

void initiateProcessManager(struct ProcessManager *manager){
    manager->highPriorityQueue = createQueue();
    manager->lowPriorityQueue = createQueue();
    manager->ioQueue = createQueue();
}

void generateProcess(struct ProcessManager *manager, int ppid, int priority, int arrivalTime, FILE *logfile){
    struct Process *process = createProcess(manager->nextPid, ppid, priority, arrivalTime);
    fprintf(stdout, C_GREEN "%s" C_RESET ": pid = %d, priority = %d, execution time = %d\n", "Process Created", process->pid, process->priority, process->burstTime);
    // printf("PROCESS CREATED: pid = %d, priority = %d, arrival time = %d, execution time = %d\n", process->pid, process->priority, process->arrivalTime, process->burstTime);
    manager->processList[manager->nextPid] = process;
    enQueue(manager->highPriorityQueue, process->pid);
    fprintf(logfile, "PROCESS CREATED -- Creation Time: %d, PID: %d, Duration: %d, PPID: %d\n",arrivalTime,process->pid,process->burstTime,ppid);
    manager->nextPid = generateNextPid(manager->nextPid);
}

int main(int argc, char **argv){
    unsigned int maxProcess;
    unsigned int timeSlice;
    unsigned int simulationTime = 0;
    unsigned int partialTime = 0;
    int opt;
    char *inputFile;
    char *outputPath;
    // struct Process *(processList[maxProcess]);
    struct Process *tmpProc;
    struct Process *teste;
    struct Node *pidRunning = NULL;
    struct Node *ioRunning = NULL;
    struct Node *newProcessCreation = NULL;
    struct Process *processRunning;
    struct Queue *processCreation = createQueue();

<<<<<<< HEAD
    FILE *ptr_logfile = NULL;
    ptr_logfile = fopen("log.txt","w");
    if(!ptr_logfile){
        printf("FALHA AO CRIAR ARQUIVO DE LOG");
        exit(1);
    }

    parseProcessFile("process.txt", processCreation);
=======
    while(1){
        int option_index = 0;
        opt = getopt_long(argc, argv, "t:l:i:o:", long_options, &option_index);
        if(opt == -1)
            break;
        switch(opt){
            case 't':
                timeSlice = atoi(optarg);
                break;
            case 'l':
                maxProcess = atoi(optarg);
                break;
            case 'i':
                inputFile = optarg;
                break;
            case 'o':
                outputPath = optarg;
                break;
            default:
                abort();

        }
    }
    struct Process *list = createProcessList(maxProcess);
    struct ProcessManager *manager = createProcessManager(maxProcess);

    parseProcessFile(inputFile, processCreation);
>>>>>>> 49f77129dcc64308cb04207ae1478c0c9157adec
    // show(processCreation);

    initiateProcessManager(manager);

    newProcessCreation = deQueue(processCreation);
    while(1){
        system("clear");
        fprintf(stdout, C_YELLOW "%s" C_RESET ": %d\n", "Simulation Time", simulationTime);
        while(newProcessCreation != NULL && newProcessCreation->data == simulationTime){
            generateProcess(manager, 0, newProcessCreation->priority, simulationTime, ptr_logfile);
            free(newProcessCreation);
            newProcessCreation = deQueue(processCreation);
            if(!newProcessCreation)
                newProcessCreation = newNode(-1);
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
            partialTime++;
            if(processRunning->burstTime == 0){
                fprintf(stdout, C_RED "%s" C_RESET ": pid = %d, priority = %d, arrival time = %d\n", "Process Terminated", processRunning->pid, processRunning->priority, processRunning->arrivalTime);
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
        }
        fprintf(stdout, C_BOLD "\n%s" C_RESET, "High Priority Queue:\t");
        show(manager->highPriorityQueue);
        fprintf(stdout, C_BOLD "%s" C_RESET, "Low Priority Queue:\t");
        show(manager->lowPriorityQueue);
        fprintf(stdout, C_BOLD "%s" C_RESET, "I/O Queue:\t\t");
        show(manager->ioQueue);
        simulationTime++;

        if(newProcessCreation->data == -1 && !pidRunning && isAllExecutionQueuesEmpty(manager) && isAllIOQueuesEmpty(manager)){
            break;
        }

        fprintf(stdout, C_WHITE "\n%s" C_RESET, "<ENTER> for next instant");
        getchar();
    }
    fprintf(stdout, C_BLUE "\n%s" C_RESET, "END OF SIMULATION\n");

    return EXIT_SUCCESS;
}