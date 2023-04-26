#ifndef SRC_READYSTATE_H
#define SRC_READYSTATE_H

typedef struct {
    int **queues; // An array of queues (arrays of integers)
    int size; // The size of each queue
} Ready;

void initReady(Ready *ready, int size);

void insertReady(Ready *ready, int processId, int prior);

void removeReady(Ready *ready, int processId);

int nextReady(Ready *ready);

void freeReady(Ready *ready);

#endif