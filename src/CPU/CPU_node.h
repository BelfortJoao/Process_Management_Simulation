#ifndef SRC_CPU_NODE_H
#define SRC_CPU_NODE_H

#include "../manager/timer.h"
#include <string.h>
#include "../process/process.h"
#include "../printer/printer.h"

#define DEFAULT_QUANTUM 8

typedef struct CPUNode
{
    Process *runningProcess; // Pointer to the process that is currently runningId
    int programCounter; //Program counter
    Timer executing_timer; //Timer for the CPU's executing time
    Timer program_timer; //Timer for the current program's time
    struct CPUNode *nextCPU;
} CPUNode;

CPUNode *initializeCPUNode(char *filename, int numberCores);

#endif //SRC_CPU_NODE_H