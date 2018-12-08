#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include <unistd.h>

#include "fifoQueues.h"
#include "formats.h"
// #include "page.h"
#include "processManager.h"
#include "process.h"
#include "parseInputFile.h"
#include "logDriver.h"

static struct option long_options[] = {
    {"auto",                no_argument         , NULL, 'a'},
    {"time_slice",          required_argument   , NULL, 't'},
    {"process_limit",       required_argument   , NULL, 'l'},
    {"input_file",          required_argument   , NULL, 'i'},
    {"output_path",         required_argument   , NULL, 'o'},
    {"disk_io_time",        required_argument   , NULL, 'd'},
    {"tape_io_time",        required_argument   , NULL, 'f'},
    {"printer_io_time",     required_argument   , NULL, 'p'},
    {"max_process_time",    required_argument   , NULL, 'm'},
    {"max_virtual_pages",   required_argument   , NULL, 'v'},
    {"help",                no_argument         , NULL, 'h'},
    {NULL, 0, NULL, 0}
};

void initiateProcessManager(struct ProcessManager *manager){
    manager->highPriorityQueue = createQueue();
    manager->lowPriorityQueue = createQueue();
    manager->ioDisk = createQueue();
    manager->ioPrinter = createQueue();
    manager->ioTape = createQueue();
}

void generateProcess(struct ProcessManager *manager, int ppid, int priority, int arrivalTime, int maxProcess, unsigned int maxProcessTime, unsigned int maxVirtualPages){
    struct Process *process = createProcess(manager->nextPid, ppid, priority, arrivalTime, maxProcessTime, maxVirtualPages);
    fprintf(stdout, C_GREEN C_BOLD "%s" C_RESET ": pid = %d, virtual pages = %d, execution time = %d\n", "Process Created", process->pid, process->pages, process->burstTime);
    manager->processList[manager->nextPid] = process;
    enQueue(manager->highPriorityQueue, process->pid);
    logProcessCreation(arrivalTime, process->pid, process->burstTime, ppid);
    manager->nextPid = generateNextPid(manager->nextPid, maxProcess);
}

void showMainMemory( unsigned int *mainMemory, unsigned int *pid ){
    fprintf(stdout, C_YELLOW C_BOLD "%s" C_RESET, "\n\nMain Memory Frames: (page/pid)\n");
    unsigned int columns = 8;
    unsigned int rows = 64 / columns;
    for( unsigned int i = 0; i < rows; i++ ){
        fprintf(stdout, C_YELLOW "%d:\t" C_RESET, i * rows );
        for( unsigned int j = 0; j < columns; j++ ){
            if( mainMemory[ ( i * rows ) + j ] == -1 )
                fprintf( stdout, C_WHITE "*\t" C_RESET );
            else
                fprintf( stdout, C_WHITE "%d/%d\t" C_RESET, mainMemory[ ( i * rows ) + j ], pid[ ( i * rows ) + j ] );
        }
        printf( "\n" );
    }
}

void initializeMainMemory( unsigned int *mainMemory, struct Queue *allocationPermited ){
    for( unsigned int i = 0; i < 64; i++ ){
        mainMemory[ i ] = -1;
        if( i % 4 == 0 )
            enQueue( allocationPermited, i );
    }
}

// unsigned int referencePage( unsigned int *mainMemory, unsigned int page ){

// }

int showHelp(){
    printf("Usage: ./sso -t <time_slice> -l <max_process> -i <input_file> -o <output_file>\n");
    printf("Params:\n");
    printf("\t-a, --auto: Execution simulation in automatic mode\n");
    printf("\t-d, --disk_io_time: Disk access time\n");
    printf("\t-f, --tape_io_time: Tape access time\n");
    printf("\t-h, --help: Show this help\n");
    printf("\t-i, --input_file: Path to the processes file\n");
    printf("\t-l, --process_limit: Maximum number of process\n");
    printf("\t-m, --max_process_time: Maximum process time\n");
    printf("\t-o, --output_path: Path to the output log file\n");
    printf("\t-p, --printer_io_time: Printer access time\n");
    printf("\t-t, --time_slice: Process time slice\n");
    printf("\t-v, --max_virtual_pages: Maximum nunber of virtual pages per process\n");
    exit(0);
}

int main(int argc, char **argv){
    time_t t;
    unsigned int maxProcess = 1024;
    unsigned int timeSlice = 4;
    unsigned int simulationTime = 0;
    unsigned int partialTime = 0;
    unsigned int diskTime = 3;
    unsigned int tapeTime = 5;
    unsigned int printerTime = 10;
    unsigned int partialDiskTime = 0;
    unsigned int partialTapeTime = 0;
    unsigned int partialPrinterTime = 0;
    unsigned int maxProcessTime = 19;
    unsigned int maxVirtualPages = 63;
    unsigned int automatic = 0;
    unsigned int mainMemoryFrames = 64;
    unsigned int mainMemory[64];
    unsigned int pidMemory[64];
    int opt;
    char *inputFile = NULL;
    char *outputFile = NULL;
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
    struct Queue *allocationPermited = createQueue();
    int option_index;
    int no_opts = 1;

    srand( (unsigned)time(&t) );

    while(1){
        option_index = 0;
        opt = getopt_long(argc, argv, "a:t:l:i:o:d:f:p:m:h:v", long_options, &option_index);
        if(opt == -1) {
            if (no_opts == 1){
                showHelp();
            }
            break;
        }
        no_opts = 0;
        switch(opt){
            case 'h':
                showHelp();
            case 'a':
                automatic = 1;
                break;
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
            case 'd':
                diskTime = atoi(optarg);
                break;
            case 'f':
                tapeTime = atoi(optarg);
                break;
            case 'p':
                printerTime = atoi(optarg);
                break;
            case 'm':
                maxProcessTime = atoi(optarg) - 1;
                break;
            case 'v':
                maxVirtualPages = atoi(optarg) - 1;
                break;
            default:
                exit(0);

        }
    }
            
    createLogFile(outputFile);

    struct ProcessManager *manager = createProcessManager(maxProcess);

    parseProcessFile(inputFile, processCreation);
    // show(processCreation);

    initiateProcessManager( manager );
    initializeMainMemory( mainMemory, allocationPermited );
    printf("ESPAÇOS DISPONIVEIS: ");
    show( allocationPermited );

    newProcessCreation = deQueue( processCreation );
    while( 1 ){
        system("clear");
        fprintf( stdout, C_YELLOW C_BOLD "%s" C_RESET ": %d\n", "Simulation Time", simulationTime );
        while( newProcessCreation != NULL && newProcessCreation->data == simulationTime ){
            generateProcess( manager, 0, newProcessCreation->priority, simulationTime, maxProcess, maxProcessTime, maxVirtualPages );
            free( newProcessCreation );
            newProcessCreation = deQueue( processCreation );
            if( !newProcessCreation )
                newProcessCreation = newNode( -1 );
        }
        // if( simulationTime % 3 == 0 ){
        //     generateProcess(manager, 0, 0, simulationTime, maxProcess, maxProcessTime, maxVirtualPages);
        // }
        if( !pidRunning ){
            if( !isEmpty( manager->highPriorityQueue ) ){
                pidRunning = deQueue( manager->highPriorityQueue );
                processRunning = manager->processList[ pidRunning->data ];
                if( processRunning->swapped || processRunning->state == 0 ){
                    if( isEmpty( allocationPermited ) ){
                        printf("NECESSARIO SWAP!");
                    }
                    else{
                        struct Node *no = deQueue( allocationPermited );
                        processRunning->baseAddress = no->data;
                        free( no );
                    }
                }
                processRunning->state = 2;
                logProcessStarted( processRunning->pid,simulationTime,processRunning->burstTime );
            }
            else if( !isEmpty( manager->lowPriorityQueue ) ){
                pidRunning = deQueue( manager->lowPriorityQueue );
                processRunning = manager->processList[ pidRunning->data ];
                processRunning->state = 2;
                logProcessStarted( processRunning->pid,simulationTime,processRunning->burstTime );
            }
        }
        if(!pidDiskRunning){
            if(!isEmpty(manager->ioDisk)){
                pidDiskRunning = deQueue(manager->ioDisk);
                processDiskRunning = manager->processList[pidDiskRunning->data];
                partialDiskTime = 0;
                logProcessStarted(pidDiskRunning->data,simulationTime,processDiskRunning->burstTime);
            }
        }
        if(!pidTapeRunning){
            if(!isEmpty(manager->ioTape)){
                pidTapeRunning = deQueue(manager->ioTape);
                processTapeRunning = manager->processList[pidTapeRunning->data];
                partialTapeTime = 0;
                logProcessStarted(pidTapeRunning->data,simulationTime,processTapeRunning->burstTime);
            }
        }
        if(!pidPrinterRunning){
            if(!isEmpty(manager->ioPrinter)){
                pidPrinterRunning = deQueue(manager->ioPrinter);
                processPrinterRunning = manager->processList[pidPrinterRunning->data];
                partialPrinterTime = 0;
                logProcessStarted(pidPrinterRunning->data,simulationTime,processPrinterRunning->burstTime);
            }
        }
        
        printf("ESPAÇOS DISPONIVEIS: ");
        show( allocationPermited );
        
        if( processRunning != NULL ){
            processRunning->burstTime -= 1;
            fprintf(stdout, C_YELLOW C_BOLD "%s" C_RESET ": pid = %d, execution time = %d, base address = %d\n", "Process Running", processRunning->pid, processRunning->burstTime, processRunning->baseAddress );
            partialTime++;
            processRunning->timeCount++;
            if ( (rand() % 10) <= 9 )//processRunning->timeCount % 3 == 0 ){
                requestPage( processRunning, mainMemory, pidMemory );
            showLRU( processRunning->lru, 4 );
            showPageTable( processRunning );
            if(processRunning->burstTime == 0){
                fprintf(stdout, C_RED C_BOLD "%s" C_RESET ": pid = %d, arrival time = %d\n", "Process Terminated", processRunning->pid, processRunning->arrivalTime);
                logProcessTerminated(processRunning->pid, simulationTime);
                free(processRunning);
                manager->processList[pidRunning->data] = NULL;
                enQueue( allocationPermited, processRunning->baseAddress );
                processRunning = NULL;
                pidRunning = NULL;
                partialTime = 0;
            }
            else if((rand() % 5) <= 1){
                if (processRunning->state != 3){
                    switch ( rand() % 3){
                        case 0:
                            /* fila do disco */
                            fprintf(stdout, C_CYAN C_BOLD "%s" C_RESET ": pid = %d\n", "Generated Disk I/O", processRunning->pid);
                            enQueue(manager->ioDisk, processRunning->pid);
                            processRunning->state = 3;
                            processRunning = NULL;
                            pidRunning = NULL;
                            partialTime = 0;
                            break;
                        case 1:
                            /* fila da fita */
                            fprintf(stdout, C_CYAN C_BOLD "%s" C_RESET ": pid = %d\n", "Generated Tape I/O", processRunning->pid);
                            enQueue(manager->ioTape, processRunning->pid);
                            processRunning->state = 3;
                            processRunning = NULL;
                            pidRunning = NULL;
                            partialTime = 0;
                            break;
                        case 2:
                            /* fila da impressora */
                            fprintf(stdout, C_CYAN C_BOLD "%s" C_RESET ": pid = %d\n", "Generated Printer I/O", processRunning->pid);
                            enQueue(manager->ioPrinter, processRunning->pid);
                            processRunning->state = 3;
                            processRunning = NULL;
                            pidRunning = NULL;
                            partialTime = 0;
                            break;
                    }
                }
            }
            else{
                if(timeSlice == partialTime){
                    // if(processRunning->priority == 0){
                    //     enQueue(manager->highPriorityQueue, processRunning->pid);
                    // }
                    // else{
                    //     enQueue(manager->lowPriorityQueue, processRunning->pid);
                    // }
                    enQueue(manager->lowPriorityQueue, processRunning->pid);
                    logProcessPreempted(processRunning->pid, simulationTime);
                    processRunning->state = 1;
                    pidRunning = NULL;
                    partialTime = 0;
                }
            }
        }
        showMainMemory( mainMemory, pidMemory );
        fprintf(stdout, C_BOLD "\n%s" C_RESET, "High Priority Queue:\t");
        show(manager->highPriorityQueue);
        fprintf(stdout, C_BOLD "%s" C_RESET, "Low Priority Queue:\t");
        show(manager->lowPriorityQueue);
        printf("\n");
        
        if(processDiskRunning != NULL){
            if(partialDiskTime < diskTime){
                fprintf(stdout, C_YELLOW C_BOLD "%s" C_RESET ": pid = %d\n", "I/O Disk Running", processDiskRunning->pid);
                partialDiskTime++;
            }
            else{
                enQueue(manager->lowPriorityQueue, pidDiskRunning->data);
                processDiskRunning->state = 1;
                partialDiskTime = 0;
                processDiskRunning = NULL;
                pidDiskRunning = NULL;
            }
        }
        if(processTapeRunning != NULL){
            if(partialTapeTime < tapeTime){
                fprintf(stdout, C_YELLOW C_BOLD "%s" C_RESET ": pid = %d\n", "I/O Tape Running", processTapeRunning->pid);
                partialTapeTime++;
            }
            else{
                enQueue(manager->highPriorityQueue, pidTapeRunning->data);
                processTapeRunning->state = 1;
                partialTapeTime = 0;
                processTapeRunning = NULL;
                pidTapeRunning = NULL;
            }
        }
        if(processPrinterRunning != NULL){
            if(partialPrinterTime < printerTime){
                fprintf(stdout, C_YELLOW C_BOLD "%s" C_RESET ": pid = %d\n", "I/O Printer Running", processPrinterRunning->pid);
                partialPrinterTime++;
            }
            else{
                enQueue(manager->lowPriorityQueue, pidPrinterRunning->data);
                processPrinterRunning->state = 1;
                partialPrinterTime = 0;
                processPrinterRunning = NULL;
                pidPrinterRunning = NULL;
            }
        }

        fprintf(stdout, C_BOLD "\n%s" C_RESET, "I/O Printer Queue:\t");
        show(manager->ioPrinter);
        fprintf(stdout, C_BOLD "%s" C_RESET, "I/O Disk Queue:\t\t");
        show(manager->ioDisk);
        fprintf(stdout, C_BOLD "%s" C_RESET, "I/O Tape Queue:\t\t");
        show(manager->ioTape);
        simulationTime++;

        if(newProcessCreation->data == -1 && !pidRunning && isAllExecutionQueuesEmpty(manager) && isAllIOQueuesEmpty(manager) && !pidDiskRunning && !pidPrinterRunning && !pidTapeRunning){
            break;
        }

        if ( automatic ){
            sleep(1);
        }
        else{
            fprintf(stdout, C_WHITE "\n%s" C_RESET, "<ENTER> for next instant");
            getchar();
        }
    }
    fprintf(stdout, C_BLUE C_BOLD "\n%s" C_RESET, "END OF SIMULATION\n");
    logSimulationEnded();

    return EXIT_SUCCESS;
}
