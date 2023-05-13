#ifndef SRC_READY_H
#define SRC_READY_H

#include <stdbool.h>
#include "queue.h"


typedef struct Ready
{
    int size;
    int maxSize;
    Queue **queues;
} Ready;


Ready *initializeReady(int size);


bool insertToReady(Ready *ready, int processId, int priority);


bool removeFromReady(Ready *ready, int processId);


int nextProcessReady(Ready *ready);


void freeReady(Ready *ready);


#endif /* SRC_READY_H */