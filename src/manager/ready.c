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
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < ready->size; j++)
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
    sortReady(ready);

    for (int i = 0; i < ready->size; i++)
    {
        if (ready->queues[prior][i] == -1)
        {
            ready->queues[prior][i] = processId;
            return;
        }
    }

    printf("Queue is full.");
}

void sortReady(Ready *ready)
{
    for (int i = 0; i < 4; ++i)
    {
        int *queue = ready->queues[i];
        int j = 0;
        while (queue[j] != -1)
        {
            int k = j + 1;
            while (queue[k] != -1)
            {
                if (queue[j] > queue[k])
                {
                    int aux = queue[j];
                    queue[j] = queue[k];
                    queue[k] = aux;
                }
                k++;
            }
            j++;
        }
    }
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

        sortReady(ready);
    }
}

void printState(Ready *ready)
{
    printf("Ready State:\n");
    for (int i = 0; i < 4; i++)
    {
        printf("Priority %d: ", i);

        for (int j = 0; j < ready->size; j++)
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