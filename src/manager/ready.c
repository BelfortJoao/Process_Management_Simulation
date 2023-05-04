#include <stdio.h>
#include <stdlib.h>
#include "../error/error.h"

#include "ready.h"

Ready *initializeReady(int size)
{
    Ready *readyProcesses = (Ready *) malloc(sizeof(Ready));

    if (!readyProcesses)
    {
        printf(ALLOCATION_ERROR, "ready processes");
        return NULL;
    }

    // Allocate Memory for the readyArray queues
    readyProcesses->queues = (int **) malloc(4 * sizeof(int *));

    if (!readyProcesses->queues)
    {
        printf(ALLOCATION_ERROR, "ready process queues");
        return NULL;
    }

    for (int i = 0; i < 4; i++)
    {
        readyProcesses->queues[i] = (int *) malloc(size * sizeof(int));

        for (int j = 0; j < size; j++)
        {
            readyProcesses->queues[i][j] = -1; // Initialize all elements to -1
        }
    }

    readyProcesses->size = size;

    return readyProcesses;
}

int nextReady(Ready *ready)
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < ready->size; ++j)
        {
            if (ready->queues[i][j] != -1)
            {
                return ready->queues[i][j];
            }
        }
    }

    return -1;
}

void insertToReadyQueue(Ready *ready, int processId, int prior)
{
    for (int k = 0; k < ready->size; ++k)
    {
        if (ready->queues[prior][k] == -1)
        {
            ready->queues[prior][k] = processId;
            //bubble sort na lista de prontos
            for (int i = 0; i < ready->size; ++i)
            {
                for (int j = 0; j < ready->size - 1; ++j)
                {
                    if (ready->queues[prior][j] > ready->queues[prior][j + 1] && ready->queues[prior][j + 1] != -1)
                    {
                        int aux = ready->queues[prior][j];
                        ready->queues[prior][j] = ready->queues[prior][j + 1];
                        ready->queues[prior][j + 1] = aux;
                    }
                }
            }

            return;
        }
    }

    printf("Ready queue full.");
}

void removeFromReadyQueue(Ready *ready, int processId)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < ready->size; j++)
        {
            if (ready->queues[i][j] == processId)
            {
                ready->queues[i][j] = -1;
            }
        }
    }
}

void printState(Ready *ready)
{
    printf("Ready State:\n");
    for (int i = 0; i < 4; ++i)
    {
        printf("Priority %d: ", i);

        for (int j = 0; j < ready->size; ++j)
        {
            printf("%d ", ready->queues[i][j]);
        }

        printf("\n");
    }
}

//TA TUDO ERRADO AQUI CONCERTO AMANHÃ

void changePriority(Ready *ready, int sourcePriority, int destinationPriority, int processId)
{
    int *sourceQueue = ready->queues[sourcePriority];
    int *destinationQueue = ready->queues[destinationPriority];

    // Find the Process index in the source queue
    int processIndex = -1;

    for (int i = 0; i < ready->size; i++)
    {
        if (sourceQueue[i] == processId)
        {
            processIndex = i;
            break;
        }
    }

    // If the Process was found in the source queue
    if (processIndex != -1)
    {
        // Remove it from the source queue
        for (int i = processIndex; i < ready->size - 1; i++)
        {
            sourceQueue[i] = sourceQueue[i + 1];
        }
        // Add it to the destination queue
        processId;
    }
}

void freeReady(Ready *ready)
{
    for (int i = 0; i < 4; i++)
    {
        free(ready->queues[i]);
    }

    free(ready->queues);
    ready->queues = NULL;
    ready->size = 0;
}