struct Process{
    int pid;
    int ppid;
    int state;
    int priority;
    int burstTime;
    int arrivalTime;
    int finishTime;
};

struct Process *createProcess(int pid, int ppid, int priority, int arrivalTime, unsigned int maxProcessTime){
    struct Process *p = (struct Process*)malloc(sizeof(struct Process));
    p->pid = pid;
    p->ppid = ppid;
    p->state = 0;
    p->priority = priority;
    p->arrivalTime = arrivalTime;
    p->burstTime = (rand() % maxProcessTime) + 1;
    return p;
}
