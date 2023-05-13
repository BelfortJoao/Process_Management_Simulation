#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "../error/error.h"

#include "queue_node.h"


QueueNode *initializeNode(int id)
{
    QueueNode *queueNode = (QueueNode *) malloc(sizeof(QueueNode));

    if (!queueNode)
    {
        printf(ALLOCATION_ERROR, "queue node");
        return NULL;
    }

    queueNode->id = id;
    queueNode->next = NULL;

    return queueNode;
}