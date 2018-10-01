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
    manager->ioDisk = createQueue();
    manager->ioPrinter = createQueue();
    manager->ioTape = createQueue();
}

void generateProcess(struct ProcessManager *manager, int ppid, int priority, int arrivalTime, int maxProcess, FILE *logfile){
    struct Process *process = createProcess(manager->nextPid, ppid, priority, arrivalTime);
    fprintf(stdout, C_GREEN "%s" C_RESET ": pid = %d, execution time = %d\n", "Process Created", process->pid, process->priority, process->burstTime);
    manager->processList[manager->nextPid] = process;
    enQueue(manager->highPriorityQueue, process->pid);
    fprintf(logfile, "PROCESS CREATED -- PID: %d, Time: %d, Duration: %d, PPID: %d\n",arrivalTime,process->pid,process->burstTime,ppid);
    manager->nextPid = generateNextPid(manager->nextPid, maxProcess);
}

int main(int argc, char **argv){
    unsigned int maxProcess;
    unsigned int timeSlice;
    unsigned int simulationTime = 0;
    unsigned int partialTime = 0;
    const unsigned int diskTime = 3;
    const unsigned int tapeTime = 5;
    const unsigned int printerTime = 10;
    unsigned int partialDiskTime = 0;
    unsigned int partialTapeTime = 0;
    unsigned int partialPrinterTime = 0;
    int opt;
    char *inputFile;
    char *outputFile;
    struct Process *tmpProc;
    struct Process *teste;
    struct Node *pidRunning = NULL;
    struct Node *pidDiskRunning = NULL;
    struct Node *pidTapeRunning = NULL;
    struct Node *pidPrinterRunning = NULL;
    struct Node *newProcessCreation = NULL;
    struct Process *processRunning = NULL;
    struct Process *processDiskRunning = NULL;
    struct Process *processTapeRunning = NULL;
    struct Process *processPrinterRunning = NULL;
    struct Queue *processCreation = createQueue();

    FILE *ptr_logfile = NULL;

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
                outputFile = optarg;
                break;
            default:
                abort();

        }
    }

    ptr_logfile = fopen(outputFile,"w");
    if(!ptr_logfile){
        printf("FALHA AO CRIAR ARQUIVO DE LOG");
        exit(1);
    }

    struct Process *list = createProcessList(maxProcess);
    struct ProcessManager *manager = createProcessManager(maxProcess);

    parseProcessFile(inputFile, processCreation);
    // show(processCreation);

    initiateProcessManager(manager);

    newProcessCreation = deQueue(processCreation);
    while(1){
        system("clear");
        fprintf(stdout, C_YELLOW "%s" C_RESET ": %d\n", "Simulation Time", simulationTime);
        while(newProcessCreation != NULL && newProcessCreation->data == simulationTime){
            generateProcess(manager, 0, newProcessCreation->priority, simulationTime, maxProcess, ptr_logfile);
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
                fprintf(ptr_logfile,"PROCESS STARTED -- PID: %d, TIME: %d, QUEUE: HIGH\n",processRunning->pid,simulationTime);
            }
            else if(!isEmpty(manager->lowPriorityQueue)){
                pidRunning = deQueue(manager->lowPriorityQueue);
                processRunning = manager->processList[pidRunning->data];
                processRunning->state = 1;
                fprintf(ptr_logfile,"PROCESS STARTED -- PID: %d, TIME: %d, QUEUE: LOW\n",processRunning->pid,simulationTime);
            }
        }
        if(!pidDiskRunning){
            if(!isEmpty(manager->ioDisk)){
                pidDiskRunning = deQueue(manager->ioDisk);
                processDiskRunning = manager->processList[pidDiskRunning->data];
                partialDiskTime = 0;
                fprintf(ptr_logfile,"PROCESS STARTED -- PID: %d, TIME: %d\n",processDiskRunning->pid,simulationTime);
            }
        }
        if(processDiskRunning != NULL){
            if(partialDiskTime < diskTime){
                fprintf(stdout, C_YELLOW "%s" C_RESET ": pid = %d\n", "I/O Disk Running", processDiskRunning->pid);
                partialDiskTime++;
            }
            else{
                enQueue(manager->lowPriorityQueue, pidDiskRunning->data);
                processDiskRunning->state = 0;
                partialDiskTime = 0;
                processDiskRunning = NULL;
                pidDiskRunning = NULL;
            }
        }
        if(!pidTapeRunning){
            if(!isEmpty(manager->ioTape)){
                pidTapeRunning = deQueue(manager->ioTape);
                processTapeRunning = manager->processList[pidTapeRunning->data];
                partialTapeTime = 0;
                fprintf(ptr_logfile,"PROCESS STARTED -- PID: %d, TIME: %d\n",processTapeRunning->pid,simulationTime);
            }
        }
        if(processTapeRunning != NULL){
            if(partialTapeTime < tapeTime){
                fprintf(stdout, C_YELLOW "%s" C_RESET ": pid = %d\n", "I/O Tape Running", processTapeRunning->pid);
                partialTapeTime++;
            }
            else{
                enQueue(manager->highPriorityQueue, pidTapeRunning->data);
                processTapeRunning->state = 0;
                partialTapeTime = 0;
                processTapeRunning = NULL;
                pidTapeRunning = NULL;
            }
        }
        if(!pidPrinterRunning){
            if(!isEmpty(manager->ioPrinter)){
                pidPrinterRunning = deQueue(manager->ioPrinter);
                processPrinterRunning = manager->processList[pidPrinterRunning->data];
                partialPrinterTime = 0;
                fprintf(ptr_logfile,"PROCESS STARTED -- PID: %d, TIME: %d\n",processPrinterRunning->pid,simulationTime);
            }
        }
        if(processPrinterRunning != NULL){
            if(partialPrinterTime < printerTime){
                fprintf(stdout, C_YELLOW "%s" C_RESET ": pid = %d\n", "I/O Printer Running", processPrinterRunning->pid);
                partialPrinterTime++;
            }
            else{
                enQueue(manager->lowPriorityQueue, pidPrinterRunning->data);
                processPrinterRunning->state = 0;
                partialPrinterTime = 0;
                processPrinterRunning = NULL;
                pidPrinterRunning = NULL;
            }
        }
        if(processRunning != NULL){
            processRunning->burstTime -= 1;
            fprintf(stdout, C_YELLOW "%s" C_RESET ": pid = %d, execution time = %d\n", "Process Running", processRunning->pid, processRunning->priority, processRunning->burstTime);
            partialTime++;
            if((rand() % 5) <= 1){
                
                switch ( rand() % 3){
                    case 0:
                        /* fila do disco */
                        enQueue(manager->ioDisk, processRunning->pid);
                        processRunning->state = 2;
                        processRunning = NULL;
                        pidRunning = NULL;
                        partialTime = 0;
                        break;
                    case 1:
                        /* fila da fita */
                        enQueue(manager->ioTape, processRunning->pid);
                        processRunning->state = 2;
                        processRunning = NULL;
                        pidRunning = NULL;
                        partialTime = 0;
                        break;
                    case 2:
                        /* fila da impressora */
                        enQueue(manager->ioPrinter, processRunning->pid);
                        processRunning->state = 2;
                        processRunning = NULL;
                        pidRunning = NULL;
                        partialTime = 0;
                        break;
                }
            }
            else{   
                if(processRunning->burstTime == 0){
                    fprintf(stdout, C_RED "%s" C_RESET ": pid = %d, arrival time = %d\n", "Process Terminated", processRunning->pid, processRunning->priority, processRunning->arrivalTime);
                    fprintf(ptr_logfile,"PROCESS TERMINATED -- PID: %d, TIME: %d\n",processRunning->pid,simulationTime);
                    free(processRunning);
                    manager->processList[pidRunning->data] = NULL;
                    processRunning = NULL;
                    pidRunning = NULL;
                    partialTime = 0;
                }
                else if(processRunning->burstTime > 0 && timeSlice == partialTime){
                    // if(processRunning->priority == 0){
                    //     enQueue(manager->highPriorityQueue, processRunning->pid);
                    // }
                    // else{
                    //     enQueue(manager->lowPriorityQueue, processRunning->pid);
                    // }
                    enQueue(manager->lowPriorityQueue, processRunning->pid);
                    fprintf(ptr_logfile,"PROCESS PREEMPTED -- PID: %d, TIME: %d\n",processRunning->pid,simulationTime);
                    processRunning->state = 0;
                    pidRunning = NULL;
                    partialTime = 0;
                }
            }
        }
        fprintf(stdout, C_BOLD "\n%s" C_RESET, "High Priority Queue:\t");
        show(manager->highPriorityQueue);
        fprintf(stdout, C_BOLD "%s" C_RESET, "Low Priority Queue:\t");
        show(manager->lowPriorityQueue);
        fprintf(stdout, C_BOLD "\n%s" C_RESET, "I/O Printer:\t\t");
        show(manager->ioPrinter);
        fprintf(stdout, C_BOLD "%s" C_RESET, "I/O Disk:\t\t");
        show(manager->ioDisk);
        fprintf(stdout, C_BOLD "%s" C_RESET, "I/O Tape:\t\t");
        show(manager->ioTape);
        simulationTime++;

        if(newProcessCreation->data == -1 && !pidRunning && isAllExecutionQueuesEmpty(manager) && isAllIOQueuesEmpty(manager)){
            break;
        }

        fprintf(stdout, C_WHITE "\n%s" C_RESET, "<ENTER> for next instant");
        getchar();
    }
    fprintf(stdout, C_BLUE "\n%s" C_RESET, "END OF SIMULATION\n");
    fprintf(ptr_logfile, "------------- SIMULATION ENDED -------------");
    fclose(ptr_logfile);

    return EXIT_SUCCESS;
}