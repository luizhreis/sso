struct ProcessManager{
    int nextPid;
    void *processArray;
    struct Queue *highPriorityQueue;
    struct Queue *lowPriorityQueue;
    struct Queue *ioQueue;
};

struct ProcessManager *createProcessManager(){
    struct ProcessManager *p = (struct ProcessManager*)malloc(sizeof(struct ProcessManager));
    // p->processArray = (struct Process*)malloc(sizeof(struct Process)*maxProcess);
    p->nextPid = 0;

    return p;
}

int generateNextPid(int currentPid){
    return ++currentPid;
}

// struct Process* generateProcess(int pid, int ppid, int priority, int quantum){
//     struct Process *process = createProcess(pid, ppid, priority, quantum);
//     return process;
// }