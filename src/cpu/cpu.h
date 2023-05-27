#ifndef SRC_CPU_H
#define SRC_CPU_H

#include "core.h"
#include "../manager/timer.h"
#include "../running/running.h"


/**
 * @brief Struct for the CPU.
 *
 * This struct contains a CPU.
 */
typedef struct
{
    Core **coreArray;
    int numberOfCores;
} CPU;


CPU *initializeCPU(int numberOfCores);

Core *nextIdleCore(CPU *cpu);

void endCPU(CPU *cpu);

Core *getCoreByRunningProcess(CPU *cpu, Process *process);

void timeUpCPU(CPU *cpu);

void freeCPU(CPU *cpu);


#endif //SRC_CPU_H