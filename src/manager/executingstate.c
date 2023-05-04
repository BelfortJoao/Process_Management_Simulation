#include <stdio.h>
#include <stdlib.h>
#include "../error/error.h"

#include "executingstate.h"

RunningProcess *initializeRunningProcess()
{
    RunningProcess *runningProcess = (RunningProcess *) malloc(sizeof(RunningProcess));

    if (!runningProcess)
    {
        printf(ALLOCATION_ERROR, "running process");
        return NULL;
    }

    return runningProcess;
}

void contextExchange(int id, RunningProcess *runningProcess)
{
    *runningProcess = id;
}