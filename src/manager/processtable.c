#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../error/error.h"

#include "processtable.h"

ProcessTable *initializeProcessTable(int initialCapacity)
{
    ProcessTable *processTable = (ProcessTable *) malloc(sizeof(ProcessTable));

    if (!processTable)
    {
        printf(ALLOCATION_ERROR, "process table");
        return NULL;
    }

    processTable->tableSize = 0;
    processTable->tableCapacity = initialCapacity;
    processTable->processArray = (Process **) malloc(initialCapacity * sizeof(Process *));

    if (!processTable->processArray)
    {
        printf(ALLOCATION_ERROR, "process table process array");
        return NULL;
    }

    processTable->programCounterArray = (int *) malloc(initialCapacity * sizeof(int));

    if (!processTable->programCounterArray)
    {
        printf(ALLOCATION_ERROR, "process table program counter array");
        return NULL;
    }

    processTable->idArray = (int *) malloc(initialCapacity * sizeof(int));

    if (!processTable->idArray)
    {
        printf(ALLOCATION_ERROR, "process table ids array");
        return NULL;
    }

    processTable->parentProcessArray = (int *) malloc(initialCapacity * sizeof(int));

    if (!processTable->parentProcessArray)
    {
        printf(ALLOCATION_ERROR, "process table parent process array");
        return NULL;
    }

    processTable->priorityIdArray = (int *) malloc(initialCapacity * sizeof(int));

    if (!processTable->priorityIdArray)
    {
        printf(ALLOCATION_ERROR, "process table priority ids array");
        return NULL;
    }

    processTable->processStateArray = (char **) malloc(initialCapacity * sizeof(char *));

    if (!processTable->processStateArray)
    {
        printf(ALLOCATION_ERROR, "process table process state array");
        return NULL;
    }

    processTable->initialTimerArray = (Timer **) malloc(initialCapacity * sizeof(Timer *));

    if (!processTable->initialTimerArray)
    {
        printf(ALLOCATION_ERROR, "process table initial time array");
        return NULL;
    }

    processTable->CPUTimerArray = (Timer **) malloc(initialCapacity * sizeof(Timer *));

    if (!processTable->CPUTimerArray)
    {
        printf(ALLOCATION_ERROR, "process table CPU time array");
        return NULL;
    }

    processTable->emptyArray = (int *) malloc(initialCapacity * sizeof(int));

    if (!processTable->emptyArray)
    {
        printf(ALLOCATION_ERROR, "process table empty array");
        return NULL;
    }

    for (int i = 0; i < initialCapacity; i++)
    {
        processTable->processArray[i] = initializeProcess();
        processTable->programCounterArray[i] = 0;
        processTable->priorityIdArray[i] = -1;
        processTable->processStateArray[i] = BLOCKED;
        processTable->emptyArray[i] = 0;
        processTable->initialTimerArray[i] = initializeTimer();
        processTable->CPUTimerArray[i] = initializeTimer();
        processTable->parentProcessArray[i] = -1;
    }

    processTable->blockedArray = initializeBlocked(initialCapacity);

    if (!processTable->blockedArray)
    {
        return NULL;
    }

    processTable->readyArray = initializeReady(initialCapacity);

    if (!processTable->readyArray)
    {
        return NULL;
    }

    processTable->executingArray = initializeRunningProcess();

    if (!processTable->executingArray)
    {
        return NULL;
    }

    processTable->nextFreeId = 0;

    contextExchange(-1, processTable->executingArray);

    return processTable;
}

int getProcessTableEmptySpace(ProcessTable *processTable)
{
    for (int i = 0; i < processTable->tableCapacity; i++)
    {
        if (processTable->emptyArray[i] == 0)
        {
            return i;
        }
    }

    return -1;
}

void addProcessTableProcess(ProcessTable *processTable, char *filename, int parentProcess, int clock)
{
    processTable->tableSize++;
    int emptyPosition = getProcessTableEmptySpace(processTable);

    processTable->emptyArray[emptyPosition] = 1;
    processTable->processArray[emptyPosition] = initializeProcessFromFile(filename);
    processTable->priorityIdArray[emptyPosition] = 0;
    processTable->processStateArray[emptyPosition] = strdup(READY);
    processTable->parentProcessArray[emptyPosition] = parentProcess;
    *processTable->initialTimerArray[emptyPosition] = clock;
    *processTable->CPUTimerArray[emptyPosition] = 0;
    processTable->idArray[emptyPosition] = nextID(processTable);

    insertToReadyQueue(processTable->readyArray,
                       emptyPosition,
                       processTable->priorityIdArray[emptyPosition]);
}

void deleteProcessTableProcess(int id, ProcessTable *processTable)
{
    int foundId = searchByIdInProcessTable(id, processTable);

    freeProcess(processTable->processArray[foundId]);
    removeFromReadyQueue(processTable->readyArray, foundId);

    processTable->tableSize--;
    processTable->emptyArray[foundId] = 0;
    processTable->processArray = NULL;
    processTable->priorityIdArray[foundId] = -1;
    processTable->programCounterArray[id] = 0;
    processTable->processStateArray[id] = strdup(BLOCKED);
    processTable->parentProcessArray[id] = -1;
    *processTable->initialTimerArray[id] = -1;
    *processTable->CPUTimerArray[id] = -1;

    removeBlockedId(processTable->blockedArray, id);
}

void deleteProcessTable(ProcessTable *processTable)
{
    for (int i = 0; i < processTable->tableCapacity; i++)
    {
        if (processTable->processArray[i] != NULL)
        {
            freeProcess(processTable->processArray[i]);
        }
    }

    freeReady(processTable->readyArray);
    freeBlocked(processTable->blockedArray);
    free(processTable->processArray);
    free(processTable->emptyArray);
    free(processTable->programCounterArray);
    free(processTable->parentProcessArray);
    free(processTable->priorityIdArray);
    free(processTable->processStateArray);
    free(processTable->initialTimerArray);
    free(processTable->CPUTimerArray);
    free(processTable);
}

int searchByIdInProcessTable(int id, ProcessTable *processTable)
{
    for (int i = 0; i < processTable->tableCapacity; i++)
    {
        if (processTable->idArray[i] == id)
        {
            return i;
        }
    }

    return -1;
}

void printProcessTable(ProcessTable *processTable)
{
    printf("Proximo idArray Livre: %d\n", processTable->nextFreeId);
    printf("Numero atual de processos: %d\n", processTable->tableSize);
    printf("Capacidade da tabela: %d\n", processTable->tableCapacity);
    printf("| %-10s | %-10s | %-10s | %-10s | %-10s | %-10s | %-10s |\n", "idArray", "PC", "Pai", "Prioridade",
           "Estado", "Inicio", "T de uso");
    printf("============================================================================================");
    for (int i = 0; i < processTable->tableCapacity; i++)
    {
        if (processTable->emptyArray[i] != 0)
        {
            if (i != 0)
            {
                printf("\n|            |            |            |            |            |            |            |");
            }
            printf("\n| %-10d | %-10d | %-10d | %-10d | %-10s | %-10d | %-10d |", processTable->idArray[i],
                   processTable->programCounterArray[i],
                   processTable->parentProcessArray[i], processTable->priorityIdArray[i],
                   processTable->processStateArray[i], *processTable->initialTimerArray[i],
                   *processTable->CPUTimerArray[i]);
        }
    }

    printf("\n");
}

int nextID(ProcessTable *processTable)
{
    processTable->nextFreeId++;
    return processTable->nextFreeId - 1;
}

void copyProcess(ProcessTable *processTable, Process *proc, Timer *timer, int PcPlus)
{
    processTable->tableSize++;
    int emptySpace = getProcessTableEmptySpace(processTable);
    processTable->emptyArray[emptySpace] = 1;
    processTable->priorityIdArray[emptySpace] = 0;
    processTable->programCounterArray[emptySpace] =
            processTable->programCounterArray[*processTable->executingArray] + PcPlus;
    processTable->processStateArray[emptySpace] = strdup(READY);
    processTable->parentProcessArray[emptySpace] = *processTable->executingArray;
    processTable->initialTimerArray[emptySpace] = timer;
    *processTable->CPUTimerArray[emptySpace] = 0;
    processTable->idArray[emptySpace] = nextID(processTable);
    insertToReadyQueue(processTable->readyArray, emptySpace, processTable->priorityIdArray[emptySpace]);
}