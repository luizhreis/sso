FILE *PTR_LOGFILE = NULL;

int createLogFile(char *outputFile){
    PTR_LOGFILE = fopen(outputFile,"w");
    if(!PTR_LOGFILE){
        fprintf(stderr, "FALHA AO CRIAR ARQUIVO DE LOG(%d): %s\n",errno, strerror(errno));
        exit(1);
    }
    return 0;
}

int logProcessStarted(int pid, int simTime, int burstTime){
    fprintf(PTR_LOGFILE,"PROCESS STARTED -- PID: %d, TIME: %d, Duration: %d\n",pid,simTime, burstTime);
    return 0;
}

int logProcessCreation(int arrivalTime, int pid, int burstTime, int ppid){
    fprintf(PTR_LOGFILE, "PROCESS CREATED -- PID: %d, Time: %d, Duration: %d, PPID: %d\n", arrivalTime, pid, burstTime, ppid);
    return 0;
}

int logProcessTerminated(int pid, int simTime){
    fprintf(PTR_LOGFILE,"PROCESS TERMINATED -- PID: %d, TIME: %d\n", pid, simTime);
    return 0;
}

int logProcessPreempted(int pid, int simTime){
    fprintf(PTR_LOGFILE,"PROCESS PREEMPTED -- PID: %d, TIME: %d\n", pid, simTime);
    return 0;
}

int logSimulationEnded(){
    fprintf(PTR_LOGFILE,"------------ END OF SIMULATION ------------");
    fclose(PTR_LOGFILE);
    return 0;
}