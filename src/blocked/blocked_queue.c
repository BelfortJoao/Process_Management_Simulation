#include <stdio.h>
#include <stdlib.h>
#include "../error/error.h"

#include "blocked_queue.h"


BlockedQueue *initializeBlockedQueue()
{
    BlockedQueue *blockedQueue = (BlockedQueue *) malloc(sizeof(BlockedQueue));

    if (!blockedQueue)
    {
        printf(ALLOCATION_ERROR, "blockedQueue");
        return NULL;
    }

    blockedQueue->front = NULL;
    blockedQueue->rear = NULL;

    return blockedQueue;
}


bool insertToBlockedQueue(BlockedQueue *blockedQueue, int id, Timer timer)
{
    if (!blockedQueue)
    {
        return false;
    }

    BlockedNode *newBlockedNode = initializeBlockedNode(id, timer);

    if (!newBlockedNode)
    {
        return false;
    }

    if (!blockedQueue->rear)
    {
        blockedQueue->front = blockedQueue->rear = newBlockedNode;
    }
    else
    {
        blockedQueue->rear->next = newBlockedNode;
        blockedQueue->rear = newBlockedNode;
    }

    return true;
}


bool removeFromBlockedQueue(BlockedQueue *blockedQueue, int id)
{
    if (!blockedQueue)
    {
        return NULL;
    }

    BlockedNode *currBlockedNode = blockedQueue->front;
    BlockedNode *previousBlockedNode = NULL;

    while (currBlockedNode)
    {
        if (currBlockedNode->id == id)
        {
            if (!previousBlockedNode)
            {
                blockedQueue->front = currBlockedNode->next;
            }
            else
            {
                previousBlockedNode->next = currBlockedNode->next;
            }

            if (!currBlockedNode->next)
            {
                blockedQueue->rear = previousBlockedNode;
            }

            free(currBlockedNode);

            return true;
        }

        previousBlockedNode = currBlockedNode;
        currBlockedNode = currBlockedNode->next;
    }

    return false;
}


void blockDownClock(BlockedQueue *blockedQueue)
{
    if (blockedQueue)
    {
        BlockedNode *currBlock = blockedQueue->front;

        while (currBlock)
        {
            BlockedNode *nextBlock = currBlock->next;
            currBlock->blockTime--;
            currBlock = nextBlock;
        }
    }
}


void freeBlockedQueue(BlockedQueue *blockedQueue)
{
    if (blockedQueue)
    {
        BlockedNode *currQueueNode = blockedQueue->front;

        while (currQueueNode)
        {
            BlockedNode *nextQueueNode = currQueueNode->next;
            free(currQueueNode);
            currQueueNode = nextQueueNode;
        }

        free(blockedQueue);
    }
}