struct ProcessManager{
    int nextPid;
    struct Queue *highPriorityQueue;
    struct Queue *lowPriorityQueue;
    struct Queue *ioQueue;
};

struct ProcessManager *createProcessManager(){
    struct ProcessManager *p = (struct ProcessManager*)malloc(sizeof(struct ProcessManager));
    p->nextPid = 0;
    p->highPriorityQueue;// = createQueue();
    p->lowPriorityQueue;// = createQueue();
    p->ioQueue;// = createQueue();

    return p;
}