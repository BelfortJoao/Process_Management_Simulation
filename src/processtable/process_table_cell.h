#ifndef SRC_PROCESS_TABLE_CELL_H
#define SRC_PROCESS_TABLE_CELL_H


#include "../process/process.h"
#include "../manager/timer.h"


typedef struct ProcessTableCell
{
    int id;
    Process *process;
    int parentProcessId;
    int programCounter;
    int priority;
    enum ProcessState state;
    Timer initialTime;
    Timer CPUTime;
} ProcessTableCell;


ProcessTableCell *initializeProcessTableCellFromFile(int id, char *filename, int parentProcess, int clock);


ProcessTableCell *copyProcessTableCell(ProcessTableCell *processTableCellToCopy, int id,
                                       int parentId, int programCounter, Timer timer);


void freeProcessTableCell(ProcessTableCell *processTableCell);


#endif //SRC_PROCESS_TABLE_CELL_H ll