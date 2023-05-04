#include <stdio.h>
#include <stdlib.h>
#include "../error/error.h"

#include "blockedstate.h"

BlockedProcesses *initializeBlockedProcesses(int size)
{
    BlockedProcesses *blockedProcesses = (BlockedProcesses *) malloc(sizeof(BlockedProcesses));

    if (!blockedProcesses)
    {
        printf(ALLOCATION_ERROR, "blocked processes");
        return NULL;
    }

    // Allocate Memory for the blockedArray array
    blockedProcesses->id = (int *) malloc(size * sizeof(int));

    if (!blockedProcesses->id)
    {
        printf(ALLOCATION_ERROR, "blocked process ids");
        return NULL;
    }

    blockedProcesses->blockTime = (int *) malloc(size * sizeof(int));

    if (!blockedProcesses->id)
    {
        printf(ALLOCATION_ERROR, "blocked process times");
        return NULL;
    }

    for (int i = 0; i < size; i++)
    {
        blockedProcesses->id[i] = -1; // Initialize all elements to -1
        blockedProcesses->blockTime[i] = -1; // Initialize all elements to -1
    }

    return blockedProcesses;
}

void insertBlockedId(BlockedProcesses *b, int pid, int blockTime)
{
    int i = 0;
    while (b->id[i] != -1)
    {
        i++;
    }
    b->id[i] = pid;
    b->blockTime[i] = blockTime;
}

void removeBlockedId(BlockedProcesses *b, int pid)
{
    int i = 0;
    while (b->id[i] != pid && i < sizeof(b->id))
    {
        i++;
    }
    if (b->id[i] == pid)
    {
        while (b->id[i] != -1 && i + 1 < sizeof(b->id))
        {
            b->id[i] = b->id[i + 1];
            b->blockTime[i] = b->blockTime[i + 1];
            i++;
        }
    }
}

void blockDownClock(BlockedProcesses *b)
{
    for (int i = 0; i < sizeof(b->id); ++i)
    {
        if (b->id[i] != -1)
        { b->blockTime[i]--; }
    }
}

void freeBlockedIds(BlockedProcesses *b)
{
    free(b->blockTime);
    free(b->id);
}