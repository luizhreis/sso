#include "fifoQueues.c"
 
struct Node *enQueue( struct Queue *, int );
struct Node *deQueue( struct Queue * );
struct Node *getLast( struct Queue * );
void show( struct Queue * );
int isEmpty( struct Queue * );