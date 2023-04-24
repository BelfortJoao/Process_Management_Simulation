//
// Created by belfort on 4/22/23.
//

#ifndef SRC_READYSTATE_H
#define SRC_READYSTATE_H
typedef struct {
    int** queues; // An array of queues (arrays of integers)
    int size; // The size of each queue
} Ready;

void initReady(Ready* r, int size);

void freeReady(Ready* r);

void insertReady(Ready* r, int pid);

void removeReady(Ready* r, int pid);

int nextReady(Ready* r);
#endif //SRC_READYSTATE_H
