//
// Created by belfort on 4/22/23.
//

#ifndef SRC_CPU_H
#define SRC_CPU_H

#include "timer.h"
#include "../Process.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct CPU{
    process* proc;
    int pc;
    timer program_timer;
    timer executing_timer;
}CPU;
void initCPU(CPU* cpu, char* arq);
void freeCPU(CPU* cpu);
int interpreter(CPU* cpu,int* blk,process* proce,char** arq);
void changeProcess(CPU* cpu,process* proc, int pc, timer program_timer, timer executing_timer);
#endif //SRC_CPU_H
