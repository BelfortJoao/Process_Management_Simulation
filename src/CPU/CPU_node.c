#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "../error/error.h"

#include "../CPU/CPU_node.h"

CPUNode *initializeCPU_Node(char *filename, int numberCores)
{
    CPUNode *cpuNODE = (CPUNode*)malloc(numberCores * sizeof(CPUNode));

    if(!cpuNODE){
        printf(ALLOCATION_ERROR, "queue cpu_node");
        return NULL;
    }

    cpuNODE->runningProcess = initializeProcessFromFile(filename);

    if(!cpuNODE->runningProcess){
        return NULL;
    }

    cpuNODE->runningProcess = (Process*) malloc(numberCores * sizeof(Process));
    cpuNODE->programCounter = 0;
    initializeTimer(&cpuNODE->executing_timer);
    initializeTimer(&cpuNODE->program_timer);
    cpuNODE->program_timer = DEFAULT_QUANTUM;
    cpuNODE->numCores = numberCores;

    return cpuNODE;

}
