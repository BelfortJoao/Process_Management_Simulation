#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../error/error.h"
#include "../printer/printer.h"

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
    initializeTimer(&cpu->executing_timer);
    initializeTimer(&cpu->program_timer);
    cpu->program_timer = DEFAULT_QUANTUM;

    return cpu;
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


int interpreter(CPU *cpu, int *blk, char **file, int *PCPlus)
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


void changeProcess(CPU *cpu, Process *process, int programCounter, Timer program_timer, Timer executing_timer)
{
    for (int i = 0; i < process->numLines; i++)
    {
        if (!cpu->runningProcess)
        {
            cpu->runningProcess = process;
        }

        strcpy(cpu->runningProcess->program[i], process->program[i]);
    }

    for (int i = 0; i < process->memorySize; i++)
    {
        cpu->runningProcess->memory[i] = process->memory[i];
    }

    cpu->programCounter = programCounter;
    cpu->program_timer = program_timer;
    cpu->executing_timer = executing_timer;

}


void freeCPU(CPU *cpu)
{
    cpu->runningProcess = NULL;
    cpu->programCounter = 0;
    cpu->executing_timer = 0;
    cpu->executing_timer = 0;
}