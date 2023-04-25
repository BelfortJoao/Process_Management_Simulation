//
// Created by belfort on 4/22/23.
//

#define MAX 100
#include "cpu.h"

int interpreter(CPU* cpu,int* blk, process* proce, char** arq, int* PCPlus){

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
            initMemory(cpu->proc,atoi(arg1));
            printf("Memory initialized with size %d\n",atoi(arg1));
            return 0;
        case 'D':
            declareVar(cpu->proc,atoi(arg1));
            printf("Var %d declarated\n",atoi(arg1));
            return 0;
        case 'V':
            changeVar(cpu->proc,atoi(arg1), atoi(arg2));
            printf("Memory pos %d changed to %d\n",atoi(arg1), atoi(arg2));
            return 0;
        case 'A':
            addVar(cpu->proc,atoi(arg1), atoi(arg2));
            printf("Memory pos %d added in %d\n",atoi(arg1), atoi(arg2));
            return 0;
        case 'S':
            subVar(cpu->proc,atoi(arg1), atoi(arg2));
            printf("Memory pos %d sub in -%d\n",atoi(arg1), atoi(arg2));
            return 0;
        case 'B':
            *blk=atoi(arg1);
            printf("Processo bloqueado por %d tempo\n",*blk);
            return 1;
        case 'T':
            printf("excluindo processo\n");
            excludeProcess(cpu->proc);
            return 2;
        case 'F':
            printf("Copiando processo\n");
            proce=generateNewProcess(cpu->proc);
            *PCPlus=atoi(arg1);
            return 3;
        case 'R':
            printf("Lendo o arquivo %s\n",arg1);
            *arq=arg1;
            return 4;
    }
}
void changeProcess(CPU* cpu,process* proc, int pc, timer program_timer, timer executing_timer){
    cpu->proc=proc;
    cpu->pc=pc;
    cpu->executing_timer=executing_timer;
    cpu->program_timer=program_timer;
};
void initCPU(CPU* cpu, char* arq) {
    cpu->proc=(process*) malloc(sizeof (process));
    initProcess(cpu->proc, arq);
    cpu->pc = 0;
    cpu->program_timer =8;
    cpu->executing_timer =0;
}

void freeCPU(CPU* cpu) {
    if (cpu->proc != NULL) {
        cpu->proc = NULL;
    }
    cpu->pc = 0;
    cpu->program_timer = 0;
    cpu->executing_timer = 0;
}
