#ifndef SRC_QUEUE_H
#define SRC_QUEUE_H

#include <stdbool.h>
#include "queue_node.h"


typedef struct Queue
{
    QueueNode *front;
    QueueNode *rear;
} Queue;


Queue *initializeQueue();


bool insertToQueue(Queue *queue, int id);


bool removeFromQueue(Queue *blockedQueue, int id);


void freeQueue(Queue *blockedQueue);


#endif // SRC_QUEUE_H