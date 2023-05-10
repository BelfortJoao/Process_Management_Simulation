#ifndef SRC_PROCESSTABLECELL_H
#define SRC_PROCESSTABLECELL_H


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


ProcessTableCell *initializeProcessTableCell();


ProcessTableCell *initializeProcessTableCellFromFile(int id, char *filename, int parentProcess, int clock);


void freeProcessTableCell(ProcessTableCell *processTableCell);


#endif //SRC_PROCESSTABLECELL_H ll