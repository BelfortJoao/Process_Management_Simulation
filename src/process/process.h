#ifndef SRC_PROCESS_H
#define SRC_PROCESS_H

typedef int *Memory;
typedef char **Program;

typedef struct {
    Memory memory; //Memoria individual que guarda as variaveis do processo
    Program program; //Programa que o processo deverá executar no CPU
    int numLines; //Numero de linhas que o programa guardado no processo contem
    int memorySize; //Tamanho da memoria do processo
} Process;

void initProcess(Process *process, char *name);

void initMemory(Process *process, int lengthMem);

void excludeProcess(Process *process);

void declareVar(Process *process, int position);

void changeVar(Process *process, int position, int value);

void addVar(Process *process, int position, int value);

void subVar(Process *process, int position, int value);

Process *generateNewProcess(Process *process);

void printMem(Process *process);

void printProg(Process *process);

#endif
