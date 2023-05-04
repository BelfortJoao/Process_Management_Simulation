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
    Timer program_timer;
    Timer executing_timer;
} CPU;

CPU *initializeCPU(char *filename);

int interpreter(CPU *cpu, int *blk, char **file, int *PcPlus);

void changeProcess(CPU *cpu, Process *process, int programCounter, Timer program_timer, Timer executing_timer);

void freeCPU(CPU *cpu);

#endif