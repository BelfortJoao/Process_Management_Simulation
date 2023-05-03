#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"

#define DEFAULT_QUANTUM 8

void initCPU(CPU *cpu, char *file)
{
    cpu->runningProcess = (Process *) malloc(sizeof(Process)); // Aloca memória apenas para a estrutura Process
    initProcess(cpu->runningProcess, file);

    cpu->programCounter = 0;
    cpu->quantum = DEFAULT_QUANTUM;
    cpu->quantumUsed = 0;
}

int interpreter(CPU *cpu, int *blk, char **arq, int *PCPlus)
{
    char *token = strtok(cpu->runningProcess->program[cpu->programCounter], " "); //Separando o input em Tokens
    char *arg1 = strtok(NULL, " "); //Separando o input em Tokens
    char *arg2 = strtok(NULL, " "); //Separando o input em Tokens

    if (arg1 == NULL)
    {
        printf("Argument 1 is null.\n");
    }

    if (arg2 == NULL)
    {
        printf("Argument 2 is null.\n");
    }

    switch (token[0])
    {
        case 'N':
            initMemory(cpu->runningProcess, atoi(arg1));
            printf("Memory initialized with size %d\n", atoi(arg1));
            return 0;
        case 'D':
            declareVar(cpu->runningProcess, atoi(arg1));
            printf("Var %d declarated\n", atoi(arg1));
            return 0;
        case 'V':
            changeVar(cpu->runningProcess, atoi(arg1), atoi(arg2));
            printf("Memory pos %d changed to %d\n", atoi(arg1), atoi(arg2));
            return 0;
        case 'A':
            addVar(cpu->runningProcess, atoi(arg1), atoi(arg2));
            printf("Memory pos %d added in %d\n", atoi(arg1), atoi(arg2));
            return 0;
        case 'S':
            subVar(cpu->runningProcess, atoi(arg1), atoi(arg2));
            printf("Memory pos %d sub in -%d\n", atoi(arg1), atoi(arg2));
            return 0;
        case 'B':
            *blk = atoi(arg1);
            printf("Blocking process for %d.\n", *blk);
            return 1;
        case 'T':
            printf("Deleting process.\n");
            excludeProcess(cpu->runningProcess);
            return 2;
        case 'F':
            printf("Copying a process.\n");
            *PCPlus = atoi(arg1);
            return 3;
        case 'R':
            printf("Reading the file %s.\n", arg1);
            *arq = arg1;
            return 4;
    }

    return 0;
}

void changeProcess(CPU *cpu, Process *proc, int pc, Timer quantum, Timer quantumUsed)
{
    for (int i = 0; i < proc->numLines; i++)
    {
        strcpy(cpu->runningProcess->program[i], proc->program[i]);
    }
    for (int i = 0; i < proc->memorySize; i++)
    {
        cpu->runningProcess->memory[i] = proc->memory[i];
    } //pode dar erro
    cpu->programCounter = pc;
    cpu->quantum = quantum;
    cpu->quantumUsed = quantumUsed;
}

void freeCPU(CPU *cpu)
{
    if (cpu->runningProcess != NULL)
    {
        cpu->runningProcess = NULL;
    }
    cpu->programCounter = 0;
    cpu->quantum = 0;
    cpu->quantumUsed = 0;
}
