#ifndef SRC_CPU_QUEUE_H
#define SRC_CPU_QUEUE_H

#include <stdbool.h>
#include "../CPU/CPU_node.h"


typedef struct CPUQueue
{
    CPUNode *front;
    CPUNode *rear;
} CPUQueue;

CPUQueue *initializeCPU(char *filename, int numberCores);

int convertStringToInt(char *string);

int interpreter(CPUNode *cpu, int *blk, char **file, int *PCPlus);

void changeProcess(CPUNode *cpuNode, Process *process, int programCounter, Timer program_timer, Timer executing_timer);

void freeCPU(CPUQueue *cpu);


#endif //SRC_CPU_QUEUE_H