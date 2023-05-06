#ifndef SRC_READY_H
#define SRC_READY_H

#include <stdbool.h>


typedef struct node
{
    int id;
    struct node *next;
} node;


typedef struct
{
    node *front;
    node *rear;
} queue;


typedef struct
{
    int size_at;
    int maxSize;
    queue queues[4];
} Ready;


Ready *initializeReady(int size);


queue *createQueue();


bool insertToReadyQueue(Ready *ready, int processId, int prior);


bool removeFromReadyQueue(Ready *ready, int processId);


int nextProcessReady(Ready *ready);


void freeReady(Ready *ready);


#endif /* SRC_READY_H */