#include <stdlib.h>
#include "running.h"

Running *initializeRunning(int numberOfRunning)
{
    Running *running = (Running *) malloc(sizeof (Running) * numberOfRunning);

    for(int i = 0; i < numberOfRunning; i++)
    {
        running[i] = -1;
    }

    return running;
}

void contextExchange(int id, Running *runningProcess, int coreNum)
{
    runningProcess[coreNum] = id;
}