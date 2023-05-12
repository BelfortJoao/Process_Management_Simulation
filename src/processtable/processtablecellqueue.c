#include <stdlib.h>
#include "../error/error.h"

#include "processtablecellqueue.h"


ProcessTableCellQueue *initializeProcessTableCellQueue()
{
    ProcessTableCellQueue *processTableCellQueue = (ProcessTableCellQueue *) malloc(sizeof(ProcessTableCellQueue));

    if (!processTableCellQueue)
    {
        printf(ALLOCATION_ERROR, "process table cell queue");
        return NULL;
    }

    processTableCellQueue->front = NULL;
    processTableCellQueue->rear = NULL;

    return processTableCellQueue;
}


bool insertToProcessTableQueue(ProcessTableCellQueue *processTableCellQueue,
                               char *filename, int parentProcessId, int clock)
{
    // ---------------------------------------- Move it later ----------------------------------------------------------
    ProcessTableCellNode *processTableCellNode = (ProcessTableCellNode *) malloc(sizeof(ProcessTableCellNode));

    if (!processTableCellNode)
    {
        printf(ALLOCATION_ERROR, "process table cell node");
        return false;
    }

    processTableCellNode->processTableCell = initializeProcessTableCellFromFile(0, filename, parentProcessId, clock);
    processTableCellNode->next = NULL;
    // -----------------------------------------------------------------------------------------------------------------

    if (!processTableCellQueue->front)
    {
        processTableCellQueue->front = processTableCellNode;
        processTableCellQueue->rear = processTableCellNode;
        return true;
    }

    processTableCellQueue->rear->next = processTableCellNode;
    processTableCellQueue->rear = processTableCellNode;

    return true;
}


bool insertCellToProcessTableQueue(ProcessTableCellQueue *processTableCellQueue, ProcessTableCell *processTableCell)
{
    // ---------------------------------------- Move it later ----------------------------------------------------------
    ProcessTableCellNode *processTableCellNode = (ProcessTableCellNode *) malloc(sizeof(ProcessTableCellNode));

    if (!processTableCellNode)
    {
        printf(ALLOCATION_ERROR, "process table cell node");
        return false;
    }

    processTableCellNode->processTableCell = processTableCell;
    processTableCellNode->next = NULL;
    // -----------------------------------------------------------------------------------------------------------------

    if (!processTableCellQueue->front)
    {
        processTableCellQueue->front = processTableCellNode;
        processTableCellQueue->rear = processTableCellNode;
        return true;
    }

    processTableCellQueue->rear->next = processTableCellNode;
    processTableCellQueue->rear = processTableCellNode;

    return true;
}


bool removeFromProcessTableQueue(ProcessTableCellQueue *processTableCellQueue, int id)
{

    if (!processTableCellQueue->front)
    {
        printf("ERROR: Process table queue is empty.");
        return false;
    }

    if (processTableCellQueue->front->processTableCell->id == id)
    {
        ProcessTableCellNode *nodeToDelete = processTableCellQueue->front;
        processTableCellQueue->front = processTableCellQueue->front->next;
        freeProcessTableCellNode(nodeToDelete);
        return true;
    }

    ProcessTableCellNode *currNode = processTableCellQueue->front;
    while (currNode->next)
    {
        if (currNode->next->processTableCell->id == id)
        {
            ProcessTableCellNode *nodeToDelete = currNode->next;
            if (currNode->next == processTableCellQueue->rear)
            {
                processTableCellQueue->rear = currNode;
            }
            currNode->next = currNode->next->next;
            freeProcessTableCellNode(nodeToDelete);
            return true;
        }

        currNode = currNode->next;
    }

    printf("ERROR: Couldn't find process to delete in process table queue.");
    return false;
}

ProcessTableCell *getProcessTableCellByProcessId(ProcessTableCellQueue *processTableCellQueue, int processId)
{
    ProcessTableCellNode *currNode = processTableCellQueue->front;

    while (currNode)
    {
        if (currNode->processTableCell->id == processId)
        {
            break;
        }

        currNode = currNode->next;
    }

    if (!currNode)
    {
        return NULL;
    }

    return currNode->processTableCell;
}


void freeProcessTableCellNode(ProcessTableCellNode *processTableCellNode)
{
    if (processTableCellNode)
    {
        freeProcessTableCell(processTableCellNode->processTableCell);
        free(processTableCellNode);
    }
}


void freeProcessTableCellQueue(ProcessTableCellQueue *processTableCellQueue)
{
    if (processTableCellQueue)
    {
        // ---------------------------------------- Move it later ------------------------------------------------------
        ProcessTableCellNode *currNode = processTableCellQueue->front;

        while (currNode)
        {
            if (!currNode->next)
            {
                break;
            }

            ProcessTableCellNode *nodeToFree = currNode;
            currNode = currNode->next;
            free(nodeToFree);
        }
        // -------------------------------------------------------------------------------------------------------------

        free(processTableCellQueue);
    }
}