#ifndef SRC_PROCESSTABLECELLQUEUE_H
#define SRC_PROCESSTABLECELLQUEUE_H

#include <stdbool.h>
#include "processtablecell.h"

typedef struct ProcessTableCellNode
{
    ProcessTableCell *processTableCell;
    struct ProcessTableCellNode *next;
} ProcessTableCellNode;

typedef struct ProcessTableCellQueue
{
    ProcessTableCellNode *front;
} ProcessTableCellQueue;


ProcessTableCellQueue *initializeProcessTableCellQueue();


bool insertToProcessTableQueue(ProcessTableCellQueue *processTableCellQueue,
                               char *filename, int parentProcessId, int clock);


bool removeFromProcessTableQueue(ProcessTableCellQueue *processTableCellQueue, int id);


ProcessTableCell *getProcessTableCellByProcessId(ProcessTableCellQueue *processTableCellQueue, int processId);


void freeProcessTableCellNode(ProcessTableCellNode *processTableCellNode);


void freeProcessTableCellQueue(ProcessTableCellQueue *processTableCellQueue);


#endif //SRC_PROCESSTABLECELLQUEUE_H