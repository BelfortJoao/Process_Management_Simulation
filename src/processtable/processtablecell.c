#include <stdio.h>
#include <stdlib.h>
#include "../error/error.h"

#include "processtablecell.h"


ProcessTableCell *initializeProcessTableCellFromFile(int id, char *filename, int parentProcess, int clock)
{
    ProcessTableCell *processTableCell = (ProcessTableCell *) malloc(sizeof(ProcessTableCell));

    if (!processTableCell)
    {
        return NULL;
    }

    processTableCell->id = id;
    processTableCell->process = initializeProcessFromFile(filename);
    processTableCell->parentProcessId = parentProcess;
    processTableCell->programCounter = 0;
    processTableCell->priority = 0;
    processTableCell->state = READY;
    processTableCell->initialTime = clock;
    initializeTimer(&processTableCell->CPUTime);

    return processTableCell;
}


ProcessTableCell *copyProcessTableCell(ProcessTableCell *processTableCellToCopy, int id,
                                       int parentId, int programCounter, Timer timer)
{
    ProcessTableCell *processTableCell = (ProcessTableCell *) malloc(sizeof(ProcessTableCell));

    if (!processTableCellToCopy)
    {
        return NULL;
    }

    processTableCell->state = READY;
    processTableCell->CPUTime = 0;
    processTableCell->initialTime = timer;
    processTableCell->id = id;
    processTableCell->parentProcessId = parentId;
    processTableCell->priority = 0;
    processTableCell->state = READY;
    processTableCell->programCounter = programCounter;
    processTableCell->process = processTableCellToCopy->process;

    return processTableCell;
}


void freeProcessTableCell(ProcessTableCell *processTableCell)
{
    if (processTableCell)
    {
        freeProcess(processTableCell->process);
        free(processTableCell);
    }
}