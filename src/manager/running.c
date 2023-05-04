#include <stdio.h>
#include <stdlib.h>
#include "../error/error.h"

#include "running.h"

Running *initializeRunningProcess()
{
    Running *runningProcess = (Running *) malloc(sizeof(Running));

    if (!runningProcess)
    {
        printf(ALLOCATION_ERROR, "running process");
        return NULL;
    }

    return runningProcess;
}

void contextExchange(int id, Running *runningProcess)
{
    *runningProcess = id;
}