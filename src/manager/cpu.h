#ifndef SRC_CPU_H
#define SRC_CPU_H

#include "timer.h"
#include "../process/process.h"
#include "processtable.h"
#include "../printer/printer.h"

typedef struct
{
    Process *runningProcess;
    int programCounter;
    Timer quantum;
    Timer quantumUsed;
} CPU;

void initCPU(CPU *cpu, char *file);

void freeCPU(CPU *cpu);

int interpreter(CPU *cpu, int *blk, char **arq, int *PcPlus);

void changeProcess(CPU *cpu, Process *proc, int pc, Timer quantum, Timer quantumUsed);

#endif