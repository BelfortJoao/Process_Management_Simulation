//
// Created by belfort on 4/22/23.
//

#ifndef SRC_CPU_H
#define SRC_CPU_H

#include "timer.h"
#include "../Process.h"

typedef struct CPU{
    struct process* proc;
    int pc;
    timer program_timer;
    timer executing_timer;
}CPU;
#endif //SRC_CPU_H
