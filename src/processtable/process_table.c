#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../error/error.h"

#include "process_table.h"


ProcessTable *initializeProcessTable(int initialCapacity, int numberOfRunning)
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
    processTable->blockedQueue = initializeBlockedQueue();

    if (!processTable->blockedQueue)
    {
        return NULL;
    }

    processTable->ready = initializeReady(initialCapacity);

    if (!processTable->ready)
    {
        return NULL;
    }

    processTable->runningId = initializeRunning(numberOfRunning);

    if (!processTable->runningId)
    {
        return NULL;
    }

    return processTable;
}


bool addProcessTableProcess(ProcessTable *processTable, Process *process, int parentProcessId, int clock)
{
    if (!insertToProcessTableQueue(processTable->processTableCellQueue, process, parentProcessId, clock))
    {
        return false;
    }

    processTable->nextFreeId++;

    return insertToReady(processTable->ready,
                         processTable->nextFreeId - 1,
                         0);
}


void deleteProcessTableProcess(int id, ProcessTable *processTable)
{
    if (!removeFromProcessTableQueue(processTable->processTableCellQueue, id))
    {
        return;
    }

    removeFromBlockedQueue(processTable->blockedQueue, id);
}


bool copyProcess(ProcessTable *processTable, Timer timer, int PcPlus, int coreNum)
{
    ProcessTableCell *processTableCell = getProcessTableCellByProcessId(processTable->processTableCellQueue,
                                                                        processTable->runningId[coreNum]);

    if (!processTableCell)
    {
        return false;
    }

    if (!insertCellToProcessTableQueue(processTable->processTableCellQueue,
                                       copyProcessTableCell(processTableCell,
                                                            processTable->nextFreeId,
                                                            processTable->runningId[coreNum],
                                                            processTableCell->programCounter,
                                                            timer)))
    {
        return false;
    }

    processTable->nextFreeId++;

    return insertToReady(processTable->ready,
                         processTable->nextFreeId - 1,
                         0);
}


void freeProcessTable(ProcessTable *processTable)
{
    if (processTable)
    {
        freeProcessTableCellQueue(processTable->processTableCellQueue);
        freeReady(processTable->ready);
        freeBlockedQueue(processTable->blockedQueue);
        free(processTable);
    }
}