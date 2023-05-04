#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../error/error.h"

#include "cpu.h"

#define DEFAULT_QUANTUM 8

CPU *initializeCPU(char *filename)
{
    CPU *cpu = (CPU *) malloc(sizeof(CPU));

    if (!cpu)
    {
        printf(ALLOCATION_ERROR, "CPU");
        return NULL;
    }

    cpu->runningProcess = initializeProcessFromFile(filename);

    if (!cpu->runningProcess)
    {
        return NULL;
    }

    cpu->programCounter = 0;
    cpu->quantum = initializeTimer();

    if (!cpu->quantum)
    {
        return NULL;
    }

    *cpu->quantum = DEFAULT_QUANTUM;

    cpu->quantumUsed = initializeTimer();

    if (!cpu->quantumUsed)
    {
        return NULL;
    }

    return cpu;
}

int convertStringToInt(char *string)
{
    char *errorCheckingStrTol;

    int number = strtol(string, &errorCheckingStrTol, 10);

    if (*errorCheckingStrTol != '\0')
    {
        printf(CONVERSION_ERROR);
        return 0;
    }

    return number;
}

int interpreter(CPU *cpu, int *blk, char **file, int *PCPlus)
{
    char *token = strtok(cpu->runningProcess->program[cpu->programCounter], " ");

    if (!token)
    {
        printf(INVALID_LINE);
        return -1;
    }

    char *firstArgument = strtok(NULL, " ");
    char *secondArgument = strtok(NULL, " ");

    switch (token[0])
    {
        case 'N':
            initializeProcessMemory(cpu->runningProcess, convertStringToInt(firstArgument));
            printf("Memory initialized with size %d.\n", convertStringToInt(firstArgument));
            return 0;
        case 'D':
            clearProcessMemory(cpu->runningProcess, convertStringToInt(firstArgument));
            printf("Memory at position %d has been cleared.\n", convertStringToInt(firstArgument));
            return 0;
        case 'V':
            changeValueInProcessMemory(cpu->runningProcess, convertStringToInt(firstArgument),
                                       convertStringToInt(secondArgument));
            printf("Memory at position %d changed to %d.\n", convertStringToInt(firstArgument),
                   convertStringToInt(secondArgument));
            return 0;
        case 'A':
            increaseValueInProcessMemory(cpu->runningProcess, convertStringToInt(firstArgument),
                                         convertStringToInt(secondArgument));
            printf("Memory increased by %d at position %d.\n", convertStringToInt(secondArgument),
                   convertStringToInt(firstArgument));
            return 0;
        case 'S':
            reduceValueInProcessMemory(cpu->runningProcess, convertStringToInt(firstArgument),
                                       convertStringToInt(secondArgument));
            printf("Memory reduced by %d at position %d.\n", convertStringToInt(secondArgument),
                   convertStringToInt(firstArgument));
            return 0;
        case 'B':
            *blk = convertStringToInt(firstArgument);
            printf("Blocking process for %d.\n", *blk);
            return 1;
        case 'T':
            printf("Deleting process.\n");
            freeProcess(cpu->runningProcess);
            return 2;
        case 'F':
            printf("Copying a process.\n");
            *PCPlus = convertStringToInt(firstArgument);
            return 3;
        case 'R':
            printf("Reading the file %s.\n", firstArgument);
            *file = firstArgument;
            return 4;
    }

    return 0;
}

void changeProcess(CPU *cpu, Process *process, int programCounter, Timer quantum, Timer quantumUsed)
{
    for (int i = 0; i < process->numLines; i++)
    {
        strcpy(cpu->runningProcess->program[i], process->program[i]);
    }

    for (int i = 0; i < process->memorySize; i++)
    {
        cpu->runningProcess->memory[i] = process->memory[i];
    } //pode dar erro

    cpu->programCounter = programCounter;
    *cpu->quantum = quantum;
    *cpu->quantumUsed = quantumUsed;
}

void freeCPU(CPU *cpu)
{
    cpu->runningProcess = NULL;
    cpu->programCounter = 0;
    cpu->quantum = 0;
    cpu->quantumUsed = 0;
}
