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

    //processTable--;
    //removeBlockedId(processTable->blockedArray, id);
}


void freeProcessTable(ProcessTable *processTable)
{
    if (processTable)
    {
        freeProcessTableCellQueue(processTable->processTableCellQueue);
        free(processTable);
    }
}


bool copyProcess(ProcessTable *processTable, Timer timer, int PcPlus)
{
    processTable->processTableCellQueue->rear->next = (ProcessTableCellNode *) malloc(sizeof(ProcessTableCellNode));
    processTable->processTableCellQueue->rear->next->next = NULL;
    processTable->processTableCellQueue->rear->next->processTableCell = (ProcessTableCell *) malloc(sizeof(ProcessTableCell));

    ProcessTableCell  *processTableCell = getProcessTableCellByProcessId(processTable->processTableCellQueue, processTable->running);
    if (!processTableCell)
    {
        return false;
    }
    processTable->processTableCellQueue->rear->next->processTableCell->CPUTime = 0;
    processTable->processTableCellQueue->rear->next->processTableCell->state = READY;
    processTable->processTableCellQueue->rear->next->processTableCell->CPUTime = 0;
    processTable->processTableCellQueue->rear->next->processTableCell->initialTime = timer;
    processTable->processTableCellQueue->rear->next->processTableCell->id = processTable->size;
    processTable->processTableCellQueue->rear->next->processTableCell->parentProcessId = processTable->running;
    processTable->processTableCellQueue->rear->next->processTableCell->priority = 0;
    processTable->processTableCellQueue->rear->next->processTableCell->state = READY;
    processTable->processTableCellQueue->rear->next->processTableCell->programCounter = processTableCell->programCounter + PcPlus;
    processTable->processTableCellQueue->rear->next->processTableCell->process = processTableCell->process;

    processTable->processTableCellQueue->rear->next->next = NULL;
    processTable->processTableCellQueue->rear = processTable->processTableCellQueue->rear->next;


    processTable->size++;

    return insertToReadyQueue(processTable->readyArray,
                              processTable->size - 1,
                              0);
}