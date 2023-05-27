#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "../error/error.h"

#include "cpu.h"

CPU *initializeCPU(int numberOfCores)
{
    CPU *cpu = (CPU *) malloc(sizeof(CPU));

    if (!cpu)
    {
        printf(ALLOCATION_ERROR, "CPU");
        return NULL;
    }

    cpu->numberOfCores = numberOfCores;
    cpu->coreArray = (Core **) malloc(numberOfCores * sizeof(Core *));

    if (!cpu->coreArray)
    {
        printf(ALLOCATION_ERROR, "Core array");
    }

    for (int i = 0; i < numberOfCores; i++)
    {
        cpu->coreArray[i] = initializeCore();
    }

    return cpu;
}


Core *getCoreByRunningProcess(CPU *cpu, Process *process)
{
    for (int i = 0; i < cpu->numberOfCores; i++)
    {
        if (cpu->coreArray[i] && cpu->coreArray[i]->coreState == WORKING &&
            cpu->coreArray[i]->runningProcess == process)
        {
            return cpu->coreArray[i];
        }
    }

    return NULL;
}


Core *nextIdleCore(CPU *cpu)
{
    for (int i = 0; i < cpu->numberOfCores; i++)
    {
        if (cpu->coreArray[i] && cpu->coreArray[i]->coreState == IDLE)
        {
            return cpu->coreArray[i];
        }
    }

    return NULL;
}

void timeUpCPU(CPU *cpu)
{
    for (int i = 0; i < cpu->numberOfCores; i++)
    {
        if (cpu->coreArray[i] && cpu->coreArray[i]->coreState == WORKING)
        {
            cpu->coreArray[i]->executing_timer++;
        }
    }
}

void endCPU(CPU *cpu)
{
    for (int i = 0; i < cpu->numberOfCores; i++)
    {
        cpu->coreArray[i]->runningProcess = NULL;
        cpu->coreArray[i]->coreState = IDLE;
    }
}


void freeCPU(CPU *cpu)
{
    if (cpu)
    {
        if (cpu->coreArray)
        {
            for (int i = 0; i < cpu->numberOfCores; i++)
            {
                if (cpu->coreArray[i])
                {
                    freeCore(cpu->coreArray[i]);
                }
            }

            free(cpu->coreArray);
        }

        free(cpu);
    }
}