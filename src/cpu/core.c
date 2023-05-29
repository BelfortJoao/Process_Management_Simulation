#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../error/error.h"
#include "../printer/printer.h"

#include "core.h"

#define DEFAULT_QUANTUM 8


Core *initializeCore()
{
    Core *core = (Core *) malloc(sizeof(Core));

    if (!core)
    {
        printf(ALLOCATION_ERROR, "Core");
        return NULL;
    }

    core->runningProcess = NULL;
    core->coreState = IDLE;
    core->programCounter = 0;
    initializeTimer(&core->executing_timer);
    initializeTimer(&core->program_timer);
    core->program_timer = DEFAULT_QUANTUM;

    return core;
}


void initializeCoreFromFile(Core *core, char *filename)
{
    core->runningProcess = initializeProcessFromFile(filename);

    if (!core->runningProcess)
    {
        return;
    }

    core->coreState = WORKING;
    core->programCounter = 0;
    initializeTimer(&core->executing_timer);
    initializeTimer(&core->program_timer);
    core->program_timer = DEFAULT_QUANTUM;
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


int interpreter(Core *core, int *blk, char **file, int *PCPlus)
{
    char *input = strdup(core->runningProcess->program[core->programCounter]);
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
            initializeProcessMemory(core->runningProcess, convertStringToInt(firstArgument));
            printInitialMemorySize(convertStringToInt(firstArgument));
            free(input);
            return 0;
        case 'D':
            clearProcessMemory(core->runningProcess, convertStringToInt(firstArgument));
            printClearMemory(convertStringToInt(firstArgument));
            free(input);
            return 0;
        case 'V':
            changeValueInProcessMemory(core->runningProcess, convertStringToInt(firstArgument),
                                       convertStringToInt(secondArgument));

            printMemoryChange(convertStringToInt(firstArgument), convertStringToInt(secondArgument));
            free(input);
            return 0;
        case 'A':
            increaseValueInProcessMemory(core->runningProcess, convertStringToInt(firstArgument),
                                         convertStringToInt(secondArgument));
            printMemoryIncrease(convertStringToInt(firstArgument), convertStringToInt(secondArgument));
            free(input);
            return 0;
        case 'S':
            reduceValueInProcessMemory(core->runningProcess, convertStringToInt(firstArgument),
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


bool changeProcess(Core *core, Process *process, int programCounter, Timer program_timer, Timer executing_timer)
{
    if (!core)
    {
        return false;
    }

    for (int i = 0; i < process->numLines; i++)
    {
        if (!core->runningProcess)
        {
            core->runningProcess = process;
        }

        strcpy(core->runningProcess->program[i], process->program[i]);
    }

    for (int i = 0; i < process->memorySize; i++)
    {
        core->runningProcess->memory[i] = process->memory[i];
    }

    core->coreState = WORKING;
    core->programCounter = programCounter;
    core->program_timer = program_timer;
    core->executing_timer = executing_timer;

    return true;
}


void freeCore(Core *core)
{
    core->runningProcess = NULL;
    core->programCounter = 0;
    core->executing_timer = 0;
    core->program_timer = 0;
}