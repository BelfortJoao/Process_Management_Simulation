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

    processTable->nextFreeId = 0;
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

    processTable->runningId = -1;

    if (!processTable->runningId)
    {
        return NULL;
    }

    contextExchange(-1, &processTable->runningId);

    return processTable;
}


bool addProcessTableProcess(ProcessTable *processTable, char *filename, int parentProcessId, int clock)
{
    if (!insertToProcessTableQueue(processTable->processTableCellQueue, filename, parentProcessId, clock))
    {
        return false;
    }

    processTable->nextFreeId++;

    return insertToReadyQueue(processTable->readyArray,
                              processTable->nextFreeId - 1,
                              0);
}


void deleteProcessTableProcess(int id, ProcessTable *processTable)
{
    if (!removeFromProcessTableQueue(processTable->processTableCellQueue, id))
    {
        return;
    }

    //removeBlockedId(processTable->blockedArray, id);
}


bool copyProcess(ProcessTable *processTable, Timer timer, int PcPlus)
{
    ProcessTableCell *processTableCell = getProcessTableCellByProcessId(processTable->processTableCellQueue,
                                                                        processTable->runningId);

    if (!processTableCell)
    {
        return false;
    }

    if (!insertCellToProcessTableQueue(processTable->processTableCellQueue,
                                       copyProcessTableCell(processTableCell,
                                                            processTable->nextFreeId,
                                                            processTable->runningId,
                                                            processTableCell->programCounter + PcPlus,
                                                            timer)))
    {
        return false;
    }

    processTable->nextFreeId++;

    return insertToReadyQueue(processTable->readyArray,
                              processTable->nextFreeId - 1,
                              0);
}


void freeProcessTable(ProcessTable *processTable)
{
    if (processTable)
    {
        freeProcessTableCellQueue(processTable->processTableCellQueue);
        free(processTable);
    }
}