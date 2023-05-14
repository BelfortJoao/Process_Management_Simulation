#ifndef SRC_CPU_QUEUE_H
#define SRC_CPU_QUEUE_H

#include <stdbool.h>
#include "../CPU/CPU_node.h"

typedef struct CPUqueue{
    CPUNode *front;
    CPUNode *rear;
}CPUqueue;

CPUqueue *initializeCPU();

int convertStringToInt(char *string);

int interpreter(CPUNode *cpu, int *blk, char **file, int *PCPlus);

void changeProcess(CPUNode *cpuNode,Process *process, int programCounter, Timer program_timer, Timer executing_timer);

void freeCPU(CPUqueue *cpu);


#endif //SRC_CPU_QUEUE_H
