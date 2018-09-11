#include <stdio.h>
#include <stdlib.h>

#include "processManager.h"

// Driver Program to test anove functions
int main(){
    struct Queue *q = createQueue();
    struct Queue *rrQueue01 = createQueue();
    struct Queue *rrQueue02 = createQueue();
    struct Queue *rrQueue03 = createQueue();
    struct Queue *cpuBoundedQueue01 = createQueue();
    struct Queue *cpuBoundedQueue02 = createQueue();
    struct Queue *ioBoundedQueue = createQueue();

    struct Process *process = createProcess(100, 0, 0, 1.5);

    struct ProcessManager *manager = createProcessManager();
    manager->highPriorityQueue = createQueue();

    printf("lista vazia: %d\n", isEmpty(manager->highPriorityQueue));
    enQueue(manager->highPriorityQueue, 10);
    printf("lista vazia: %d\n", isEmpty(manager->highPriorityQueue));
    show(manager->highPriorityQueue);
    enQueue(manager->highPriorityQueue, 20);
    printf("lista vazia: %d\n", isEmpty(manager->highPriorityQueue));
    show(manager->highPriorityQueue);
    deQueue(manager->highPriorityQueue);
    show(manager->highPriorityQueue);
    deQueue(manager->highPriorityQueue);
    show(manager->highPriorityQueue);
    enQueue(manager->highPriorityQueue, 30);
    show(manager->highPriorityQueue);
    enQueue(manager->highPriorityQueue, 40);
    show(manager->highPriorityQueue);
    enQueue(manager->highPriorityQueue, 50);
    show(manager->highPriorityQueue);

    printf("PID: %d, QUANTUM: %f\n", process->pid, process->quantum);

    struct Node *n = deQueue(manager->highPriorityQueue);
    if (n != NULL)
      printf("Dequeued item is %d\n", n->data);
    return 0;
}