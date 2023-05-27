#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "../error/error.h"

#include "../CPU/CPU_node.h"


CPUNode *initializeCPUNode(char *filename, int numberCores)
{
    CPUNode *cpuCore = (CPUNode *) malloc(numberCores * sizeof(CPUNode));

    if (!cpuCore)
    {
        printf(ALLOCATION_ERROR, "queue cpu_node");
        return NULL;
    }

    cpuCore->runningProcess = initializeProcessFromFile(filename);

    if (!cpuCore->runningProcess)
    {
        return NULL;
    }

    cpuCore->runningProcess = (Process *) malloc(numberCores * sizeof(Process));
    cpuCore->programCounter = 0;
    initializeTimer(&cpuCore->executing_timer);
    initializeTimer(&cpuCore->program_timer);
    cpuCore->program_timer = DEFAULT_QUANTUM;

    return cpuCore;
}