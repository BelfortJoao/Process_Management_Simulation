#include <stdio.h>
#include <stdlib.h>
#include "../error/error.h"

#include "blocked.h"


Blocked *initializeBlocked(int size)
{
    Blocked *blockedProcesses = (Blocked *) malloc(sizeof(Blocked));

    if (!blockedProcesses)
    {
        printf(ALLOCATION_ERROR, "blocked processes");
        return NULL;
    }

    // Allocate Memory for the blockedArray array
    blockedProcesses->ids = (int *) malloc(size * sizeof(int));

    if (!blockedProcesses->ids)
    {
        printf(ALLOCATION_ERROR, "blocked process ids");
        return NULL;
    }

    blockedProcesses->blockTimes = (int *) malloc(size * sizeof(int));

    if (!blockedProcesses->ids)
    {
        printf(ALLOCATION_ERROR, "blocked process times");
        return NULL;
    }

    blockedProcesses->size = size;

    for (int i = 0; i < size; i++)
    {
        blockedProcesses->ids[i] = -1; // Initialize all elements to -1
        blockedProcesses->blockTimes[i] = -1; // Initialize all elements to -1
    }

    return blockedProcesses;
}


void insertBlockedId(Blocked *blocked, int processId, int blockTime)
{
    int i = 0;

    while (blocked->ids[i] != -1)
    {
        i++;
    }

    blocked->ids[i] = processId;
    blocked->blockTimes[i] = blockTime;
}


void removeBlockedId(Blocked *blocked, int processId)
{
    int i = 0;

    while (blocked->ids[i] != processId && i < blocked->size)
    {
        i++;
    }

    if (blocked->ids[i] == processId)
    {
        while (blocked->ids[i] != -1 && i + 1 < blocked->size)
        {
            blocked->ids[i] = blocked->ids[i + 1];
            blocked->blockTimes[i] = blocked->blockTimes[i + 1];
            i++;
        }
    }
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
    free(blocked->blockTimes);
    free(blocked->ids);
}