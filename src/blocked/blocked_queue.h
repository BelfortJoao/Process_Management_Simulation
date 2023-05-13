#ifndef SRC_BLOCKED_QUEUE_H
#define SRC_BLOCKED_QUEUE_H

#include <stdbool.h>
#include "blocked_node.h"


typedef struct BlockedQueue
{
    BlockedNode *front;
    BlockedNode *rear;
} BlockedQueue;


BlockedQueue *initializeBlockedQueue();


bool insertToBlockedQueue(BlockedQueue *blockedQueue, int id, Timer);


bool removeFromBlockedQueue(BlockedQueue *blockedQueue, int id);


void blockDownClock(BlockedQueue *blockedQueue);


void freeBlockedQueue(BlockedQueue *blockedQueue);


#endif //SRC_BLOCKED_QUEUE_H