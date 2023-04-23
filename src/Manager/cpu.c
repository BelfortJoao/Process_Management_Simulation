//
// Created by belfort on 4/22/23.
//


#include "cpu.h"

void interpreter(CPU* cpu){
    printf("____interpretando linha %s\n",cpu->proc->prog[cpu->pc]);
    process* proc2;
    char* token=  strtok(cpu->proc->prog[cpu->pc], " "); //Separando o input em Tokens
    char* arg1= strtok(NULL, " "); //Separando o input em Tokens
    char* arg2= strtok(NULL, " "); //Separando o input em Tokens
    if(arg1==NULL){
        printf("argumento 1 nulo\n");
    }
    if(arg2==NULL){
        printf("argumento 2 nulo\n");
    }
    switch (token[0]) {
        case 'N':
            printf("%s",arg1);
            initMemory(cpu->proc,atoi(arg1));
            printf("Memory initialized with size %d\n",atoi(arg1));
            cpu->pc++;
            return;
        case 'D':
            token=  strtok(cpu->proc->prog[cpu->pc], " ");
            declareVar(cpu->proc,atoi(token));
            printf("Var %d declarated\n",atoi(token));
            cpu->pc++;
            return;
        case 'V':
            changeVar(cpu->proc,atoi(arg1), atoi(arg2));
            printf("Memory pos %d changed to %d\n",atoi(arg1), atoi(arg2));
            cpu->pc++;
            return;
        case 'A':
            addVar(cpu->proc,atoi(arg1), atoi(arg2));
            printf("Memory pos %d added in %d\n",atoi(arg1), atoi(arg2));
            cpu->pc++;
            return;
        case 'S':
            subVar(cpu->proc,atoi(arg1), atoi(arg2));
            printf("Memory pos %d sub in -%d\n",atoi(arg1), atoi(arg2));
            cpu->pc++;
            return;
        case 'B':
            printf("tamo trabaiano nisso\n");
            cpu->pc++;
            return;
        case 'T':
            printf("excluindo processo\n");
            excludeProcess(cpu->proc);
            cpu->pc++;
            return;
        case 'F':
            printf("Copiando processo\n");
            proc2=generateNewProcess(cpu->proc);
            cpu->pc++;
            return;
        case 'R':
            printf("tamo trabaiano nisso 2\n");
            cpu->pc++;
            return;
    }
}
void changeProcess(CPU* cpu,process* proc, int pc, timer program_timer, timer executing_timer){
    cpu->proc=proc;
    cpu->pc=pc;
    cpu->executing_timer=executing_timer;
    cpu->program_timer=program_timer;
};
void initCPU(CPU* cpu) {
    cpu->proc = NULL;
    cpu->pc = 0;
    cpu->program_timer = 0;
    cpu->executing_timer = 0;
}

void freeCPU(CPU* cpu) {
    if (cpu->proc != NULL) {
        cpu->proc = NULL;
    }
    cpu->pc = 0;
    cpu->program_timer = 0;
    cpu->executing_timer = 0;
}
