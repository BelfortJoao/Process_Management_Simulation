#ifndef SRC_BLOCKED_H
#define SRC_BLOCKED_H

#include <stdbool.h>
#include "../datastruct/queue.h"

typedef struct
{
    //int *blockTimes;
    //int *ids;
    int size;
    int maxSize;
    Queue **BLOCKqueues;
} Blocked; // Processos que estão atualmente bloqueados


Blocked *initializeBlocked(int size);


//void insertBlockedId(Blocked *blocked, int processId, int blockTime);
void insertToBlockedQueue(Blocked *blocked, int processId, int blockTime);

bool removeBlockedId(Blocked *blocked, int processId);


void blockDownClock(Blocked *blocked);


void freeBlocked(Blocked *blocked);


#endif /* SRC_BLOCKED_H */