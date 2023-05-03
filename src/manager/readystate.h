#ifndef SRC_READYSTATE_H
#define SRC_READYSTATE_H

typedef struct
{
    int **queues; // An array of queues (arrays of integers)
    int size; // The size of each queue
} ReadyProcesses;

void initReady(ReadyProcesses *ready, int size);

void insertToReadyQueue(ReadyProcesses *ready, int processId, int prior);

void removeFromReadyQueue(ReadyProcesses *ready, int processId);

int nextReady(ReadyProcesses *ready);

void freeReady(ReadyProcesses *ready);

void printState( Ready *ready);
#endif