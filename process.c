struct Process{
    int pid;
    int ppid;
    int state;
    int priority;
    float quantum;
};

struct Proccess *createProcess(int pid, int ppid, int priority, float quantum){
    struct Process *p = (struct Process*)malloc(sizeof(struct Process));
    p->pid = pid;
    p->ppid = ppid;
    p->state = 0;
    p->quantum = quantum;
};
