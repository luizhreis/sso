#include "process.c"

void requestPage( struct Process *p , unsigned int *mainMemory, unsigned int *pidMemory );
int findLRU( int *lru, int n );
void showLRU( int *lru, int n );
void showPageTable( struct Process *p );