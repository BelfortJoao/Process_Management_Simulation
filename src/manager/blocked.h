#ifndef SRC_BLOCKED_H
#define SRC_BLOCKED_H

#include <stdbool.h>
#include "timer.h"

typedef struct blockNode
{
    int id;
    Timer blocked_time;
    struct QueueNode *next;
} BlockNode;

typedef struct Blocked
{
    BlockNode *front;
    BlockNode *rear;
} Blocked;

Blocked *initializeBlocked();
BlockNode *initializeBlockedkNode(int id, Timer time);
bool insertBlockedId(Blocked *blocked, int processId, int blockTime);


bool removeBlockedId(Blocked *blocked, int processId);


void blockDownClock(Blocked *blocked);


void freeBlocked(Blocked *blocked);


#endif /* SRC_BLOCKED_H */