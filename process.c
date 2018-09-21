struct Process{
    int pid;
    int ppid;
    int state;
    int priority;
    int burstTime;
    int arrivalTime;
    int finishTime;
};

struct Proccess *createProcess(int pid, int ppid, int priority, int arrivalTime){
    struct Process *p = (struct Process*)malloc(sizeof(struct Process));
    p->pid = pid;
    p->ppid = ppid;
    p->state = 0;
    p->priority = priority;
    p->arrivalTime = arrivalTime;
    p->burstTime = (rand() % 19) + 1;
};
