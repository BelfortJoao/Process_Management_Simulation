#ifndef SRC_READY_H
#define SRC_READY_H

typedef struct
{
    int **queues; // An array of queues (arrays of integers)
    int size; // The size of each queue
} Ready;

Ready *initializeReady(int size);

void insertToReadyQueue(Ready *ready, int processId, int prior);

void removeFromReadyQueue(Ready *ready, int processId);

int nextReady(Ready *ready);

void freeReady(Ready *ready);

void printState(Ready *ready);

#endif