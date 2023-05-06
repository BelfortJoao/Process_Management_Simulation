#include <stdlib.h>
#include <string.h>

#include "cpu.h"

int interpreter(CPU *cpu, int *blk, char **arq, int *PCPlus) {
    char *input = strdup(cpu->proc->program[cpu->pc]); //Faz uma cópia da string de entrada para evitar alterá-la
    char *token = strsep(&input, " "); //Separando o input em Tokens
    char *arg1 = strsep(&input, " "); //Separando o input em Tokens
    char *arg2 = strsep(&input, " "); //Separando o input em Tokens
    switch (token[0]) {
        case 'N':
            initMemory(cpu->proc, atoi(arg1));
            printInitMem(arg1);
            free(input); //Libera a memória alocada por strdup()
            return 0;
        case 'D':
            declareVar(cpu->proc, atoi(arg1));
            printDeclareVar(arg1);
            free(input); //Libera a memória alocada por strdup()
            return 0;
        case 'V':
            changeVar(cpu->proc, atoi(arg1), atoi(arg2));
            printChangeVar(arg1, arg2);
            free(input); //Libera a memória alocada por strdup()
            return 0;
        case 'A':
            addVar(cpu->proc, atoi(arg1), atoi(arg2));
            printAddVar(arg1, arg2);
            free(input); //Libera a memória alocada por strdup()
            return 0;
        case 'S':
            subVar(cpu->proc, atoi(arg1), atoi(arg2));
            printSubVar(arg1, arg2);
            free(input); //Libera a memória alocada por strdup()
            return 0;
        case 'B':
            *blk = atoi(arg1);
            printBlkProcess(arg1);
            free(input); //Libera a memória alocada por strdup()
            return 1;
        case 'T':
            printTerProcess();
            free(input); //Libera a memória alocada por strdup()
            return 2;
        case 'F':
            printCopyProcess();
            *PCPlus = atoi(arg1);
            free(input); //Libera a memória alocada por strdup()
            return 3;
        case 'R':
            printReadFile(arg1);
            *arq = arg1;
            free(input); //Libera a memória alocada por strdup()
            return 4;
    }
    free(input); //Libera a memória alocada por strdup()
    return -1;
}

void changeProcess(CPU *cpu, Process *proc, int pc, Timer program_timer, Timer executing_timer) {
    for (int i = 0; i < proc->numLines; i++) {
        strcpy(cpu->proc->program[i], proc->program[i]);
    }
    for (int i = 0; i < proc->memorySize; i++) {
        cpu->proc->memory[i] = proc->memory[i];
    } //pode dar erro
    cpu->pc = pc;
    cpu->program_timer = program_timer;
    cpu->executing_timer = executing_timer;
};

void initCPU(CPU *cpu, char *arq) {
    cpu->proc = malloc(sizeof(Process)); // Aloca memória apenas para a estrutura Process
    initProcess(cpu->proc, arq);
    cpu->pc = 0;
    cpu->program_timer = 8;
    cpu->executing_timer = 0;
}

void freeCPU(CPU *cpu) {
    if (cpu->proc != NULL) {
        cpu->proc = NULL;
    }
    cpu->pc = 0;
    cpu->program_timer = 0;
    cpu->executing_timer = 0;
}
