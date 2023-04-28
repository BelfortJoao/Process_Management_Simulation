#ifndef SRC_CPU_H
#define SRC_CPU_H

#include "timer.h"
#include "../process/process.h"

typedef struct CPU {
    Process *proc;
    int pc;
    Timer program_timer;
    Timer executing_timer;
} CPU;

void initCPU(CPU *cpu, char *arq);

void freeCPU(CPU *cpu);

int interpreter(CPU *cpu, int *blk, Process *process, char **arq, int *PcPlus);

void changeProcess(CPU *cpu, Process *proc, int pc, Timer program_timer, Timer executing_timer);

#endif