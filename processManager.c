struct ProcessManager{
    int nextPid;
    // struct Process *(processList[1024]);
    struct Queue *highPriorityQueue;
    struct Queue *lowPriorityQueue;
    struct Queue *ioQueue;
    struct Process *(processList)[];
};

struct Process *createProcessList(unsigned int maxProcess){
    struct Process *processList = (struct Process*) malloc(sizeof(struct Process*) * maxProcess);
    return processList;
}

struct ProcessManager *createProcessManager(struct Process *processList){
    struct ProcessManager *p = (struct ProcessManager*)malloc(sizeof(struct ProcessManager) + sizeof(processList));
    // p->processList = (struct Process*)malloc(sizeof(struct Process)*maxProcess);
    p->nextPid = 0;

    return p;
}

int generateNextPid(int currentPid){
    return ++currentPid;
}

int isAllExecutionQueuesEmpty(struct ProcessManager *manager){
    if(isEmpty(manager->highPriorityQueue) && isEmpty(manager->lowPriorityQueue)){
        return 1;
    }
    return 0;
}

int isAllIOQueuesEmpty(struct ProcessManager *manager){
    if(isEmpty(manager->ioQueue)){
        return 1;
    }
    return 0;
}

// struct Process* generateProcess(int pid, int ppid, int priority, int burstTime){
//     struct Process *process = createProcess(pid, ppid, priority, burstTime);
//     return process;
// }