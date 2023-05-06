#ifndef SRC_READY_H
#define SRC_READY_H

#include <stdbool.h>
#include "../datastruct/queue.h"

typedef struct
{
    int size;
    int maxSize;
    Queue **queues;
} Ready;


Ready *initializeReady(int size);


bool insertToReadyQueue(Ready *ready, int processId, int prior);


bool removeFromReadyQueue(Ready *ready, int processId);


int nextProcessReady(Ready *ready);


void freeReady(Ready *ready);


#endif /* SRC_READY_H */