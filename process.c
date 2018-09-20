struct Process{
    int pid;
    int ppid;
    int state;
    int priority;
    int burstTime;
};

struct Proccess *createProcess(int pid, int ppid, int priority, int burstTime){
    struct Process *p = (struct Process*)malloc(sizeof(struct Process));
    p->pid = pid;
    p->ppid = ppid;
    p->state = 0;
    p->burstTime = burstTime;
};
