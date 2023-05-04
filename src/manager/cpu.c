#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../error/error.h"

#include "cpu.h"

#define DEFAULT_QUANTUM 8

CPU *initializeCPU(char *filename) {
    CPU *cpu = (CPU *) malloc(sizeof(CPU));

    if (!cpu) {
        printf(ALLOCATION_ERROR, "CPU");
        return NULL;
    }

    cpu->runningProcess = initializeProcessFromFile(filename);

    if (!cpu->runningProcess) {
        return NULL;
    }

    cpu->programCounter = 0;
    initializeTimer(&cpu->executing_timer);
    initializeTimer(&cpu->program_timer);
    cpu->program_timer = DEFAULT_QUANTUM;

    return cpu;
}

int convertStringToInt(char *string) {
    char *errorCheckingStrTol;

    int number = strtol(string, &errorCheckingStrTol, 10);

    if (*errorCheckingStrTol != '\0') {
        printf(CONVERSION_ERROR);
        return 0;
    }

    return number;
}

int interpreter(CPU *cpu, int *blk, char **file, int *PCPlus) {
    char *input = strdup(cpu->runningProcess->program[cpu->programCounter]);
    char *token = strsep(&input, " ");

    if (!token) {
        printf(INVALID_LINE);
        return -1;
    }

    char *firstArgument = strsep(&input, " ");
    char *secondArgument = strsep(&input, " ");

    switch (token[0]) {
        case 'N':
            initializeProcessMemory(cpu->runningProcess, convertStringToInt(firstArgument));
            printf("Memory initialized with size %d.\n", convertStringToInt(firstArgument));
            free(input);
            return 0;
        case 'D':
            clearProcessMemory(cpu->runningProcess, convertStringToInt(firstArgument));
            printf("Memory at position %d has been cleared.\n", convertStringToInt(firstArgument));
            free(input);
            return 0;
        case 'V':
            changeValueInProcessMemory(cpu->runningProcess, convertStringToInt(firstArgument),
                                       convertStringToInt(secondArgument));
            printf("Memory at position %d changed to %d.\n", convertStringToInt(firstArgument),
                   convertStringToInt(secondArgument));
            free(input);
            return 0;
        case 'A':
            increaseValueInProcessMemory(cpu->runningProcess, convertStringToInt(firstArgument),
                                         convertStringToInt(secondArgument));
            printf("Memory increased by %d at position %d.\n", convertStringToInt(secondArgument),
                   convertStringToInt(firstArgument));
            free(input);
            return 0;
        case 'S':
            reduceValueInProcessMemory(cpu->runningProcess, convertStringToInt(firstArgument),
                                       convertStringToInt(secondArgument));
            printf("Memory reduced by %d at position %d.\n", convertStringToInt(secondArgument),
                   convertStringToInt(firstArgument));
            free(input);
            return 0;
        case 'B':
            *blk = convertStringToInt(firstArgument);
            printf("Blocking process for %d.\n", *blk);
            free(input);
            return 1;
        case 'T':
            printf("Deleting process.\n");
//            freeProcess(cpu->runningProcess);
            free(input);
            return 2;
        case 'F':
            printf("Copying a process.\n");
            *PCPlus = convertStringToInt(firstArgument);
            free(input);
            return 3;
        case 'R':
            printf("Reading the file %s.\n", firstArgument);
            *file = firstArgument;
            free(input);
            return 4;
    }

    free(input);
    return 0;
}

void changeProcess(CPU *cpu, Process *process, int programCounter, Timer program_timer, Timer executing_timer) {
    for (int i = 0; i < process->numLines; i++) {
        strcpy(cpu->runningProcess->program[i], process->program[i]);
    }

    for (int i = 0; i < process->memorySize; i++) {
        cpu->runningProcess->memory[i] = process->memory[i];
    } //pode dar erro

    cpu->programCounter = programCounter;
    cpu->program_timer = program_timer;
    cpu->executing_timer = executing_timer;
}

void freeCPU(CPU *cpu) {
    cpu->runningProcess = NULL;
    cpu->programCounter = 0;
    cpu->executing_timer = 0;
    cpu->executing_timer = 0;
}
