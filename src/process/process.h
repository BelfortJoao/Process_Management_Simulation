#ifndef SRC_PROCESS_H
#define SRC_PROCESS_H

typedef int *Memory;
typedef char **Program;

typedef struct
{
    Memory memory; //Memoria individual que guarda as variaveis do processo
    Program program; //Programa que o processo deverá executar no CPU
    int numLines; //Numero de linhas que o programa guardado no processo contem
    int memorySize; //Tamanho da memoria do processo
} Process;

Process *initializeProcess();

Process *initializeProcessFromFile(char *filename);

void initializeProcessMemory(Process *process, int memorySize);

Process *generateProcessCopy(Process *processToCopy);

void clearProcessMemory(Process *process, int position);

void changeValueInProcessMemory(Process *process, int position, int value);

void increaseValueInProcessMemory(Process *process, int position, int value);

void reduceValueInProcessMemory(Process *process, int position, int value);

void freeProcess(Process *process);

void printProcessMemoryInfo(Process *process);

void printProcessInfo(Process *process);

#endif
