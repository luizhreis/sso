struct ProcessManager{
    int nextPid;
    struct Queue *highPriorityQueue;
    struct Queue *lowPriorityQueue;
    struct Queue *ioQueue;
};

struct ProcessManager *createProcessManager(){
    struct ProcessManager *p = (struct ProcessManager*)malloc(sizeof(struct ProcessManager));
    p->nextPid = 0;

    return p;
}

int generatePid(int currentPid){
    return currentPid++;
}
