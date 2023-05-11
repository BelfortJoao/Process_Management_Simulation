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

    if (processTableCellQueue->front)
    {
        ProcessTableCellNode *currNode = processTableCellQueue->front;

        while (currNode)
        {
            if (!currNode->next)
            {
                break;
            }

            currNode = currNode->next;
        }

        currNode->next = processTableCellNode;
    }
    else
    {
        processTableCellQueue->front = processTableCellNode;
    }

    return true;
}


bool removeFromProcessTableQueue(ProcessTableCellQueue *processTableCellQueue, int id)
{
    if (processTableCellQueue->front)
    {
        printf("ERROR: Couldn't find process to delete in process table queue.");
        return false;
    }

    ProcessTableCellNode *currNode = processTableCellQueue->front;

    while (currNode)
    {
        if (currNode->next && currNode->next->processTableCell->id == id)
        {
            break;
        }

        currNode = currNode->next;
    }

    if (!currNode->next || currNode->next->processTableCell->id != id)
    {
        printf("ERROR: Couldn't find process to delete in process table queue.");
        return false;
    }

    ProcessTableCellNode *nodeToDelete = currNode->next;
    currNode->next = currNode->next->next;
    freeProcessTableCellNode(nodeToDelete);

    return true;
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