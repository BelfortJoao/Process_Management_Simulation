//
// Created by belfort on 4/22/23.
//

#ifndef SRC_CPU_H
#define SRC_CPU_H

#include "timer.h"
#include "../Process.h"

typedef struct CPU{
    struct process* program;
    int pc;
    int* memory;
    time program_timer;
    time executing_timer;
};
#endif //SRC_CPU_H
