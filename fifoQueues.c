// A linked list (LL) node to store a queue entry
struct Node{
    int data;
    int priority;
    struct Node *next;
};
 
// The queue, front stores the front node of LL and rear stores ths
// last node of LL
struct Queue{
    struct Node *first, *last;
};
 
// A utility function to create a new linked list node.
struct Node* newNode(int k){
    struct Node *temp = (struct Node*)malloc(sizeof(struct Node));
    temp->data = k;
    temp->next = NULL;
    return temp; 
}
 
// A utility function to create an empty queue
struct Queue *createQueue(){
    struct Queue *q = (struct Queue*)malloc(sizeof(struct Queue));
    q->first = NULL;
    q->last = NULL;
    return q;
}

// The function to add a key k to q
struct Node *enQueue(struct Queue *q, int k){
    // Create a new LL node
    struct Node *temp = newNode(k);
 
    // If queue is empty, then new node is front and rear both
    if (q->last == NULL)
    {
       q->first = q->last = temp;
       return temp;
    }
 
    // Add the new node at the end of queue and change rear
    q->last->next = temp;
    q->last = temp;

    return temp;
}
 
// Function to remove a key from given queue q
struct Node *deQueue(struct Queue *q){
    // If queue is empty, return NULL.
    if (q->first == NULL)
       return NULL;
 
    // Store previous front and move front one node ahead
    struct Node *temp = q->first;
    q->first = q->first->next;
 
    // If front becomes NULL, then change rear also as NULL
    if (q->first == NULL)
       q->last = NULL;
    return temp;
}

int isEmpty(struct Queue *q){
    int ret = 0;
    if(!q->first && !q->last)
        ret = 1;
    return ret;
}

void show(struct Queue *q){
    if(isEmpty(q)){
        printf("Lista vazia");
    } else {
        struct Node *temp = q->first;
        do{
            printf("%d ", temp->data);
            temp = temp->next;
        }
        while(temp);
    }
    printf("\n");
}
