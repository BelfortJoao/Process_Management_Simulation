#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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

    processTable->processTableCellQueue = initializeProcessTableCellQueue();

    if (!processTable->processTableCellQueue)
    {
        return NULL;
    }

    processTable->size = 0;
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

    processTable->running = -1;

    if (!processTable->running)
    {
        return NULL;
    }

    contextExchange(-1, &processTable->running);

    return processTable;
}


bool addProcessTableProcess(ProcessTable *processTable, char *filename, int parentProcessId, int clock)
{
    if (!insertToProcessTableQueue(processTable->processTableCellQueue, filename, parentProcessId, clock))
    {
        return false;
    }

    processTable->size++;

    return insertToReadyQueue(processTable->readyArray,
                              processTable->size - 1,
                              0);
}


void deleteProcessTableProcess(int id, ProcessTable *processTable)
{
    if (!removeFromProcessTableQueue(processTable->processTableCellQueue, id))
    {
        return;
    }

    processTable--;
    removeBlockedId(processTable->blockedArray, id);
}


void freeProcessTable(ProcessTable *processTable)
{
    if (processTable)
    {
        freeProcessTableCellQueue(processTable->processTableCellQueue);
        free(processTable);
    }
}


//bool copyProcess(ProcessTable *processTable, Timer timer, int PcPlus)
//{
//    processTable->size++;
//
//    int emptySpace = getProcessTableEmptySpace(processTable);
//    processTable->emptyArray[emptySpace] = 1;
//    processTable->priorityIdArray[emptySpace] = 0;
//    processTable->programCounterArray[emptySpace] =
//            processTable->programCounterArray[*processTable->running] + PcPlus;
//    processTable->processStateArray[emptySpace] = READY;
//    processTable->parentProcessArray[emptySpace] = *processTable->running;
//    processTable->initialTimerArray[emptySpace] = timer;
//    processTable->CPUTimerArray[emptySpace] = 0;
//    processTable->idArray[emptySpace] = nextID(processTable);
//
//    return insertToReadyQueue(processTable->readyArray,
//                              processTable->size - 1,
//                              0);
//}