#ifndef SRC_QUEUE_NODE_H
#define SRC_QUEUE_NODE_H

#include <stdbool.h>


typedef struct QueueNode
{
    int id;
    struct QueueNode *next;
} QueueNode;


QueueNode *initializeNode(int id);


#endif //SRC_QUEUE_NODE_H