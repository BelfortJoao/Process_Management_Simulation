#ifndef SRC_CPU_H
#define SRC_CPU_H

#include "CPU_queue.h"

typedef struct CPU
{
    CPUQueue *cpuCoresQueue;
    int numberOfCores;
} CPU;


#endif
