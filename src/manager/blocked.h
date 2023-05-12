#ifndef SRC_BLOCKED_H
#define SRC_BLOCKED_H

#include <stdbool.h>
#include "timer.h"

typedef struct blockNode
{
    int id;
    Timer blocked_time;
    struct QueueNode *next;
} blockNode;

typedef struct Blocked
{
    blockNode *front;
    blockNode *rear;
} Blocked;

Blocked *initializeBlocked(int size);


void insertBlockedId(Blocked *blocked, int processId, int blockTime);


void removeBlockedId(Blocked *blocked, int processId);


void blockDownClock(Blocked *blocked);


void freeBlocked(Blocked *blocked);


#endif /* SRC_BLOCKED_H */