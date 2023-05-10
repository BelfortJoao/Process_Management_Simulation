#include <stdio.h>
#include <stdlib.h>
#include "../error/error.h"

#include "processtablecell.h"


ProcessTableCell *initializeProcessTableCell()
{
    ProcessTableCell *processTableCell = (ProcessTableCell *) malloc(sizeof(ProcessTableCell));

    if (!processTableCell)
    {
        printf(ALLOCATION_ERROR, "process table cell");
        return NULL;
    }

    processTableCell->id = -1;
    processTableCell->process = initializeProcess();
    processTableCell->parentProcessId - 1;
    processTableCell->programCounter = 0;
    processTableCell->priority = -1;
    processTableCell->state = BLOCKED;
    initializeTimer(&processTableCell->initialTime);
    initializeTimer(&processTableCell->CPUTime);

    return processTableCell;
}


ProcessTableCell *initializeProcessTableCellFromFile(int id, char *filename, int parentProcess, int clock)
{
    ProcessTableCell *processTableCell = initializeProcessTableCell();

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


void freeProcessTableCell(ProcessTableCell *processTableCell)
{
    if (processTableCell)
    {
        freeProcess(processTableCell->process);
        free(processTableCell);
    }
}