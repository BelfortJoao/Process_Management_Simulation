#ifndef SRC_QUEUENODE_H
#define SRC_QUEUENODE_H

#include <stdbool.h>


typedef struct QueueNode
{
    int id;
    int BLOCKTimes;
    struct QueueNode *next;
} QueueNode;


QueueNode *initializeQueueNode(int id);


#endif //SRC_QUEUENODE_H
