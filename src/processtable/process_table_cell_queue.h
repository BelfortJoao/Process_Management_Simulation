#ifndef SRC_PROCESS_TABLE_CELL_QUEUE_H
#define SRC_PROCESS_TABLE_CELL_QUEUE_H

#include <stdbool.h>
#include "process_table_cell.h"

typedef struct ProcessTableCellNode
{
    ProcessTableCell *processTableCell;
    struct ProcessTableCellNode *next;
} ProcessTableCellNode;

typedef struct ProcessTableCellQueue
{
    ProcessTableCellNode *front;
    ProcessTableCellNode *rear;
} ProcessTableCellQueue;


ProcessTableCellQueue *initializeProcessTableCellQueue();


bool insertToProcessTableQueue(ProcessTableCellQueue *processTableCellQueue,
                               char *filename, int parentProcessId, int clock);

bool insertCellToProcessTableQueue(ProcessTableCellQueue *processTableCellQueue, ProcessTableCell *processTableCell);


bool removeFromProcessTableQueue(ProcessTableCellQueue *processTableCellQueue, int id);


ProcessTableCell *getProcessTableCellByProcessId(ProcessTableCellQueue *processTableCellQueue, int processId);


void freeProcessTableCellNode(ProcessTableCellNode *processTableCellNode);


void freeProcessTableCellQueue(ProcessTableCellQueue *processTableCellQueue);


#endif //SRC_PROCESS_TABLE_CELL_QUEUE_H