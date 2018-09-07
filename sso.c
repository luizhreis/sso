#include <stdio.h>
#include <stdlib.h>

#include "fifoQueues.h"
#include "process.h"

// Driver Program to test anove functions
int main(){
    struct Queue *q = createQueue();
    struct Queue *rrQueue01 = createQueue();
    struct Queue *rrQueue02 = createQueue();
    struct Queue *rrQueue03 = createQueue();
    struct Queue *cpuBoundedQueue01 = createQueue();
    struct Queue *cpuBoundedQueue02 = createQueue();
    struct Queue *ioBoundedQueue = createQueue();

    struct Process *process = createProcess(100, 1.5);

    printf("lista vazia: %d\n", isEmpty(q));
    enQueue(q, 10);
    printf("lista vazia: %d\n", isEmpty(q));
    show(q);
    enQueue(q, 20);
    printf("lista vazia: %d\n", isEmpty(q));
    show(q);
    deQueue(q);
    show(q);
    deQueue(q);
    show(q);
    enQueue(q, 30);
    show(q);
    enQueue(q, 40);
    show(q);
    enQueue(q, 50);
    show(q);

    printf("PID: %d, QUANTUM: %f\n", process->pid, process->quantum);

    struct Node *n = deQueue(q);
    if (n != NULL)
      printf("Dequeued item is %d\n", n->data);
    return 0;
}