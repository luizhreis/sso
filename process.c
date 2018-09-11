struct Process{
    int pid;
    int ppid;
    int state;
    float quantum;
};

struct Proccess *createProcess(int pid, int ppid, float quantum){
    struct Process *p = (struct Process*)malloc(sizeof(struct Process));
    p->pid = pid;
    p->ppid = ppid;
    p->state = 0;
    p->quantum = quantum;
};
