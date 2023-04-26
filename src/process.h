#ifndef SRC_PROCESS_H
#define SRC_PROCESS_H
#endif

typedef int *memory;
typedef char **program;
typedef struct process {
    memory mem; //Memoria individual que guarda as variaveis do processo
    program prog; //Programa que o processo deverá executar no CPU
    int numLines; //Numero de linhas que o programa guardado no processo contem
    int lengthMem; //Tamanho da memoria do processo
} Process;

void initProcess(Process *proc, char *name);

void initMemory(Process *proc, int lengthMem);

void excludeProcess(Process *proc);

void declareVar(Process *proc, int position);

void changeVar(Process *proc, int possition, int value);

void addVar(Process *proc, int possition, int value);

void subVar(Process *proc, int possition, int value);

Process *generateNewProcess(Process *proc);

void printMem(Process *proc);

void printProg(Process *proc);
