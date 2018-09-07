struct Process{
    int pid;
    int state;
    int pidFather;
    float quantum;
};

struct Proccess *createProcess(int pid, float quantum){
    struct Process *p = (struct Process*)malloc(sizeof(struct Process));
    p->pid = pid;
    p->state = 0;
    p->quantum = quantum;
};
