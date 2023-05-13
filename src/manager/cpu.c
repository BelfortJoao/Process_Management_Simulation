#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../error/error.h"
#include "../printer/printer.h"

#include "cpu.h"

#define DEFAULT_QUANTUM 8

CPU *initializeCPU()
{
    CPU *cpu = (CPU*)malloc(sizeof(CPU));

    if(!cpu){
        printf(ALLOCATION_ERROR, "queue");
        return NULL;
    }
    cpu->front = NULL;
    cpu->rear = NULL;

    return cpu;
}

CPUNode *initializeCPU_Node(char *filename, int numberCores)
{
    CPUNode *cpuNODE = (CPUNode*)malloc(sizeof(CPUNode));

    if(!cpuNODE){
        printf(ALLOCATION_ERROR, "queue cpu_node");
        return NULL;
    }

    cpuNODE->runningProcess = initializeProcessFromFile(filename);

    if(!cpuNODE->runningProcess){
        return NULL;
    }

    cpuNODE->runningProcess = (Process**) malloc(numberCores * sizeof(Process*));
    cpuNODE->programCounters = (int*) malloc(numberCores * sizeof(int));
    cpuNODE->executing_timer = (Timer*) malloc(numberCores * sizeof(Timer));
    cpuNODE->program_timer = (Timer*) malloc(numberCores * sizeof(Timer));
    cpuNODE->numCores = numberCores;

    if (!cpuNODE->runningProcess || !cpuNODE->programCounters || !cpuNODE->executing_timer || !cpuNODE->program_timer)
    {
        printf(ALLOCATION_ERROR, "CPU arrays");
        free(cpuNODE);
        return NULL;
    }

    for (int i = 0; i < numberCores; i++)
    {
        cpuNODE->runningProcess[i] = initializeProcessFromFile(filename);
        cpuNODE->programCounters[i] = 0;
        initializeTimer(&cpuNODE->executing_timer[i]);
        initializeTimer(&cpuNODE->program_timer[i]);
        cpuNODE->program_timer[i] = DEFAULT_QUANTUM;
    }

    return cpuNODE;

}


int convertStringToInt(char *string)
{
    char *errorCheckingStrTol;

    int number = (int) strtol(string, &errorCheckingStrTol, 10);

    if (*errorCheckingStrTol != '\0')
    {
        printf(CONVERSION_ERROR);
        return 0;
    }

    return number;
}


int interpreter(CPUNode *cpuNode, int *blk, char **file, int *PCPlus)
{
    char *input = strdup(cpuNode->runningProcess->program[cpu->programCounter]);
    char *token = strsep(&input, " ");

    if (!token)
    {
        printf(INVALID_LINE);
        return -1;
    }

    char *firstArgument = strsep(&input, " ");
    char *secondArgument = strsep(&input, " ");

    switch (token[0])
    {
        case 'N':
            initializeProcessMemory(cpu->runningProcess, convertStringToInt(firstArgument));
            printInitialMemorySize(convertStringToInt(firstArgument));
            free(input);
            return 0;
        case 'D':
            clearProcessMemory(cpu->runningProcess, convertStringToInt(firstArgument));
            printClearMemory(convertStringToInt(firstArgument));
            free(input);
            return 0;
        case 'V':
            changeValueInProcessMemory(cpu->runningProcess, convertStringToInt(firstArgument),
                                       convertStringToInt(secondArgument));

            printMemoryChange(convertStringToInt(firstArgument), convertStringToInt(secondArgument));
            free(input);
            return 0;
        case 'A':
            increaseValueInProcessMemory(cpu->runningProcess, convertStringToInt(firstArgument),
                                         convertStringToInt(secondArgument));
            printMemoryIncrease(convertStringToInt(firstArgument), convertStringToInt(secondArgument));
            free(input);
            return 0;
        case 'S':
            reduceValueInProcessMemory(cpu->runningProcess, convertStringToInt(firstArgument),
                                       convertStringToInt(secondArgument));
            printDecreaseMemory(convertStringToInt(firstArgument), convertStringToInt(secondArgument));
            free(input);
            return 0;
        case 'B':
            *blk = convertStringToInt(firstArgument);
            printBlockedProcess(convertStringToInt(firstArgument));
            free(input);
            return 1;
        case 'T':
            printFinishProcess();
            free(input);
            return 2;
        case 'F':
            printCopyProcess();
            *PCPlus = convertStringToInt(firstArgument);
            free(input);
            return 3;
        case 'R':
            printReadFile(firstArgument);
            *file = firstArgument;
            free(input);
            return 4;
    }

    free(input);
    return 0;
}


void changeProcess(CPUNode *cpuNode, Process *process, int programCounter, Timer program_timer, Timer executing_timer)
{
    for (int i = 0; i < process->numLines; i++)
    {
        if(!cpuNode->runningProcess){
            cpuNode->runningProcess=process;
        }
        strcpy(cpuNode->runningProcess->program[i], process->program[i]);
    }

    for (int i = 0; i < process->memorySize; i++)
    {
        cpuNode->runningProcess->memory[i] = process->memory[i];
    }

    cpuNode->programCounters = programCounter;
    cpuNode->program_timer = program_timer;
    cpuNode->executing_timer = executing_timer;
}

void freeCPU(CPU *cpu)
{
    if(cpu){
        CPUNode *currentQueueNode = cpu->front;

        while(currentQueueNode){
            CPUNode *nextQueueNode = currentQueueNode->nextCPU;
            free(currentQueueNode);
            currentQueueNode = nextQueueNode;
        }

        free(cpu);
    }
}
