#include <stdio.h>
#include <stdlib.h>
#include "../error/error.h"

#include "blocked.h"
#define DEFAULT_QUEUE_SIZE 1


Blocked *initializeBlocked(int size)
{
    Blocked *blockedProcesses = (Blocked *) malloc(sizeof(Blocked));

    if (!blockedProcesses)
    {
        printf(ALLOCATION_ERROR, "blocked processes");
        return NULL;
    }

    blockedProcesses->size = 0;
    blockedProcesses->maxSize = size;

    blockedProcesses->BLOCKqueues = (Queue**)malloc(DEFAULT_QUEUE_SIZE * sizeof(Queue *));

    if(!blockedProcesses->BLOCKqueues){
        printf(ALLOCATION_ERROR, "array of queues");
        return NULL;
    }

    for(int i = 0; i < DEFAULT_QUEUE_SIZE; i++) {
        blockedProcesses->BLOCKqueues[i] = initializeQueue();
    }

    return blockedProcesses;

}


void insertToBlockedQueue(Blocked *blocked, int processId, int blockTime)
{
    insertToQueue(blocked->BLOCKqueues[blockTime], processId);


}


bool removeBlockedId(Blocked *blocked, int processId)
{
    return false;
}


void blockDownClock(Blocked *blocked)
{
    for (int i = 0; i < blocked->size; ++i)
    {
        if (blocked->ids[i] != -1)
        {
            blocked->blockTimes[i]--;
        }

    }
}


void freeBlocked(Blocked *blocked)
{
    if(blocked){
        for(int i = 0; i < DEFAULT_QUEUE_SIZE; i++){
            freeQueue(blocked->BLOCKqueues[i]);
        }
        if(blocked->BLOCKqueues) {
            free(blocked->BLOCKqueues);
        }
        free(blocked);
    }
}