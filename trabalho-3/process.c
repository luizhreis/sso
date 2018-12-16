struct Process{
    int pid;
    int ppid;
    int state;
    int priority;
    int burstTime;
    int timeCount;
    int arrivalTime;
    int finishTime;
    int pages;
    int workingSetLimit;
    int swapped;
    unsigned int baseAddress;
    int *pageTable;
    int *valid;
    int *modified;
    int *lru;
    int *loaded;
};

struct Process *createProcess( int pid, int ppid, int priority, int arrivalTime, unsigned int maxProcessTime, unsigned int maxVirtualPages ){
    struct Process *p = ( struct Process* )malloc( sizeof( struct Process ) );
    p->baseAddress = -1;
    p->pid = pid;
    p->ppid = ppid;
    p->state = 0;
    p->priority = priority;
    p->arrivalTime = arrivalTime;
    p->burstTime = (rand() % maxProcessTime) + 1;
    p->pages = (rand() % maxVirtualPages) + 1;
    p->timeCount = 0;
    p->workingSetLimit = 4;
    p->swapped = 0;
    p->pageTable = ( int * )malloc( sizeof( int ) * maxVirtualPages );
    p->valid = ( int * )malloc( sizeof( int ) * maxVirtualPages );
    p->modified = ( int * )malloc( sizeof( int ) * maxVirtualPages );
    p->lru = ( int * )malloc(sizeof( int ) * 4 );
    p->loaded = ( int * )malloc(sizeof( int ) * 4 );
    for( int i = 0; i < p->pages; i++ ){
        p->pageTable[ i ] = -1;
        p->valid[ i ] = 0;
        p->modified[ i ] = 0;
    }
    for( int i = 0; i < 4; i++ ){
        p->lru[ i ] = -1;
        p->loaded[ i ] = -1;
    }
    return p;
}

void requestPage( struct Process *p , unsigned int *mainMemory, unsigned int *pidMemory ){
    int page = rand() % ( p->pages ) + 1;
    fprintf(stdout, C_CYAN C_BOLD "%s " C_RESET "%d\t\n", "Page Requested:", page);
    if( p->valid[ page - 1 ] ){
        for( int i = 0; i < 4; i++ ){
            if( page == p->loaded[ i ] ){
                p->lru[ i ] = 0;
            }
            else{
                if( p->lru[ i ] != -1 )
                    p->lru[ i ] += 1;
            }
        }
    }
    else{
        int last = 0, pos;
        for( int i = 0; i < 4; i++ ){
            if( p->lru[ i ] == -1 ){
                pos = i;
                break;
            }
            if( last <= p->lru[ i ] ){
                last = p->lru[ i ];
                pos = i;
            }
        }
        for( int i = 0; i < 4; i++ ){
            if( pos != i ){
                if( p->lru[ i ] != -1 )
                    p->lru[ i ] += 1;
            }
        }
        int mem = p->loaded[ pos ];
        p->loaded[ pos ] = page - 1;
        p->lru[ pos ] = 0;
        p->valid[ mem ] = 0;
        p->valid[ page - 1 ] = 1;
        // printf("DEBUG: baseaddress = %d, pos = %d\n", p->baseAddress, pos);
        mainMemory[ p->baseAddress + pos ] = page;
        pidMemory[ p->baseAddress + pos ] = p->pid;
        p->pageTable[ page -1 ] = p->baseAddress + pos;
    }
}

void showLRU( int *lru, int n ){
    int i, tmp, j, *v;
    v = ( int * )malloc(sizeof( int ) * n );
    for( int k = 0; k < n; k++ ){
        v[ k ] = lru[ k ];
    }
    for( i = 0; i < n; i ++ ){
        for( j = 0; j < n - 1; j++ ){
            if( ( *( v + j ) ) == ( *( v + j + 1 ) ) ){
                tmp = *( v + j );
                *( v + j ) = *( v + j + 1 );
                *( v + j + 1 ) = tmp;
            }
        }
    }
    fprintf(stdout, C_YELLOW C_BOLD "%s" C_RESET, "LRU: ");
    for( int k = 0; k < n; k++ ){
        if( v[ k ] == -1 )
            printf("* " );
        else
            printf("%d ", v[ k ] );
    }
}

void showPageTable( struct Process *p ){
    fprintf(stdout, C_YELLOW C_BOLD "%s %d: " C_RESET "(frame/valid bit)\n", "\n\nPage Table - pid", p->pid);
    int flag = 0;
    int columns = 8;
    int rows = 64 / columns;
    for( int i = 0; i < rows; i++ ){
        fprintf(stdout, C_YELLOW "%d:\t" C_RESET, i * rows );
        for( int j = 0; j < columns; j++ ){
            if( p->pageTable[ ( i * rows ) + j ] == -1 )
                if( p->valid[ ( i * rows ) + j ] )
                    fprintf( stdout, C_WHITE "*/*\t" C_RESET );
                else
                    fprintf( stdout, C_WHITE "*/ \t" C_RESET );
            else
                if( p->valid[ ( i * rows ) + j ] )
                    fprintf( stdout, C_WHITE "%d/*\t" C_RESET, p->pageTable[ ( i * rows ) + j ] );
                else
                    fprintf( stdout, C_WHITE "%d/ \t" C_RESET, p->pageTable[ ( i * rows ) + j ] );
            if( ( i * rows ) + j + 1 == p->pages ){
                flag = 1;
                break;
            }
        }
        printf( "\n" );
        if( flag )
            break;
    }
}
