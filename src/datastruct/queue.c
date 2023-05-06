#include <stdio.h>
#include <stdlib.h>
#include "../error/error.h"

#include "queue.h"

Queue *initializeQueue()
{
    Queue *queue = (Queue *) malloc(sizeof(Queue));

    if (!queue)
    {
        printf(ALLOCATION_ERROR, "queue");
        return NULL;
    }

    queue->front = NULL;
    queue->rear = NULL;

    return queue;
}

bool insertToQueue(Queue *queue, int id)
{
    if (!queue)
    {
        return false;
    }

    QueueNode *newQueueNode = initializeQueueNode(id);

    if (!newQueueNode)
    {
        return false;
    }

    if (!queue->rear)
    {
        queue->front = queue->rear = newQueueNode;
    }
    else
    {
        queue->rear->next = newQueueNode;
        queue->rear = newQueueNode;
    }

    return true;
}


bool removeFromQueue(Queue *queue, int id)
{
    if (!queue)
    {
        return NULL;
    }

    QueueNode *currQueueNode = queue->front;
    QueueNode *previousQueueNode = NULL;

    while (currQueueNode)
    {
        if (currQueueNode->id == id)
        {
            if (!previousQueueNode)
            {
                queue->front = currQueueNode->next;
            }
            else
            {
                previousQueueNode->next = currQueueNode->next;
            }

            if (!currQueueNode->next)
            {
                queue->rear = previousQueueNode;
            }

            free(currQueueNode);

            return true;
        }

        previousQueueNode = currQueueNode;
        currQueueNode = currQueueNode->next;
    }

    return false;
}


void freeQueue(Queue *queue)
{
    if (queue)
    {
        QueueNode *currQueueNode = queue->front;

        while (currQueueNode)
        {
            QueueNode *nextQueueNode = currQueueNode->next;
            free(currQueueNode);
            currQueueNode = nextQueueNode;
        }

        free(queue);
    }
}