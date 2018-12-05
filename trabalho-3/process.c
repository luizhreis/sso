struct Process{
    int pid;
    int ppid;
    int state;
    int priority;
    int burstTime;
    int timeCount;
    int arrivalTime;
    int finishTime;
    int pages;
};

struct Process *createProcess(int pid, int ppid, int priority, int arrivalTime, unsigned int maxProcessTime, unsigned int maxVirtualPages){
    struct Process *p = (struct Process*)malloc(sizeof(struct Process));
    p->pid = pid;
    p->ppid = ppid;
    p->state = 0;
    p->priority = priority;
    p->arrivalTime = arrivalTime;
    p->burstTime = (rand() % maxProcessTime) + 1;
    p->pages = (rand() % maxVirtualPages) + 1;
    p->timeCount = 0;
    return p;
}
