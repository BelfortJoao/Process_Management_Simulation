#include <stdio.h>
#include <stdlib.h>
#include "../error/error.h"

#include "blocked.h"


BlockNode *initializeBlockedkNode(int id, Timer time)
{
    BlockNode *block =(BlockNode *) malloc(sizeof(BlockNode));

        if (!block)
        {
            printf(ALLOCATION_ERROR, "queue node");
            return NULL;
        }

        block->id = id;
        block->blocked_time =(Timer) time;
        block->next = NULL;
        return block;
}

Blocked *initializeBlocked()
{
    Blocked *block = (Blocked *) malloc(sizeof(Blocked));

    if (!block)
    {
        printf(ALLOCATION_ERROR, "queue");
        return NULL;
    }

    block->front = NULL;
    block->rear = NULL;

    return block;
}


bool insertBlockedId(Blocked *blocked, int processId, int blockTime){
    if (!blocked)
    {
        return false;
    }
    BlockNode *newblock= initializeBlockedkNode(processId, blockTime);
    if (!newblock)
    {
        return false;
    }

    if (!blocked->rear)
    {
        blocked->front = blocked->rear = newblock;
    }
    else
    {
        blocked->rear->next = newblock;
        blocked->rear = newblock;
    }
    return true;
}


bool removeBlockedId(Blocked *blocked, int processId)
{
    if (!blocked)
    {
        return NULL;
    }

    BlockNode *currBlockedNode = blocked->front;
    BlockNode *previousBlockedNode = NULL;

    while (currBlockedNode)
    {
        if (currBlockedNode->id == processId)
        {
            if (!previousBlockedNode)
            {
                blocked->front = currBlockedNode->next;
            }
            else
            {
                previousBlockedNode->next = currBlockedNode->next;
            }

            if (!currBlockedNode->next)
            {
                blocked->rear = previousBlockedNode;
            }

            free(currBlockedNode);

            return true;
        }

        previousBlockedNode = currBlockedNode;
        currBlockedNode = currBlockedNode->next;
    }

    return false;
}


void blockDownClock(Blocked *blocked)
{
    if(blocked) {
        BlockNode *crrblock = blocked->front;
        while (crrblock) {
            BlockNode *nextblock = crrblock->next;
            crrblock->blocked_time--;
            crrblock = nextblock;
        }
    }
}


void freeBlocked(Blocked *blocked)
{
    if (blocked)
    {
        BlockNode *currQueueNode = blocked->front;

        while (currQueueNode)
        {
            BlockNode *nextQueueNode = currQueueNode->next;
            free(currQueueNode);
            currQueueNode = nextQueueNode;
        }

        free(blocked);
    }
}