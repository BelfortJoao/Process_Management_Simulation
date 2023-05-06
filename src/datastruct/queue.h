#ifndef SRC_QUEUE_H
#define SRC_QUEUE_H

#include <stdbool.h>
#include "queuenode.h"

typedef struct
{
    QueueNode *front;
    QueueNode *rear;
} Queue;


Queue *initializeQueue();


bool insertToQueue(Queue *queue, int id);


bool removeFromQueue(Queue *queue, int id);


void freeQueue(Queue *queue);


#endif //SRC_QUEUE_H
