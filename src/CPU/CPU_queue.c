#include <stdio.h>
#include <stdlib.h>
#include "../CPU/CPU_queue.h"

#include "../error/error.h"


CPUQueue *initializeCPU(char *filename, int numberCores)
{
    CPUQueue *cpu_queue = (CPUQueue *) malloc(sizeof(CPUQueue));

    if (!cpu_queue)
    {
        printf(ALLOCATION_ERROR, "queue");
        return NULL;
    }

    CPUNode *newCPUNode = initializeCPUNode(filename, numberCores);

    cpu_queue->front = newCPUNode;
    cpu_queue->rear = NULL;

    return cpu_queue;
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


int interpreter(CPUNode *cpu, int *blk, char **file, int *PCPlus)
{
    char *input = strdup(cpu->runningProcess->program[cpu->programCounter]);
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
        if (!cpuNode->runningProcess)
        {
            cpuNode->runningProcess = process;
        }
        strcpy((cpuNode->runningProcess->program[i]), process->program[i]);
    }

    for (int i = 0; i < process->memorySize; i++)
    {
        cpuNode->runningProcess->memory[i] = process->memory[i];
    }

    cpuNode->programCounter = programCounter;
    cpuNode->program_timer = program_timer;
    cpuNode->executing_timer = executing_timer;
}


void freeCPU(CPUQueue *cpu)
{
    if (cpu)
    {
        CPUNode *currentQueueNode = cpu->front;

        while (currentQueueNode)
        {
            CPUNode *nextQueueNode = currentQueueNode->nextCPU;
            free(currentQueueNode);
            currentQueueNode = nextQueueNode;
        }

        free(cpu);
    }
}