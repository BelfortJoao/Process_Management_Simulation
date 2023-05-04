#ifndef SRC_CPU_H
#define SRC_CPU_H

#include "timer.h"
#include "../process/process.h"

typedef struct
{
    Process *runningProcess;
    int programCounter;
    Timer *quantum;
    Timer *quantumUsed;
} CPU;

CPU *initializeCPU(char *filename);

int interpreter(CPU *cpu, int *blk, char **file, int *PcPlus);

void changeProcess(CPU *cpu, Process *process, int programCounter, Timer quantum, Timer quantumUsed);

void freeCPU(CPU *cpu);

#endif