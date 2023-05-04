#ifndef SRC_PROCESSTABLE_H
#define SRC_PROCESSTABLE_H

#include "../process/process.h"
#include "timer.h"
#include "ready.h"
#include "blocked.h"
#include "running.h"

typedef struct
{
    int nextFreeId; // proximo idArray livre
    int tableSize; // número de processos na tabela
    int tableCapacity; // capacidade maxima da tabela
    int *emptyArray; // vetor de flags para espaços vazios
    int *idArray; //vetor de ids de processos de acordo com suas posições
    Process **processArray; // vetor de ponteiros para processos
    int *programCounterArray; // vetor de contadores de programa para cada processo
    int *parentProcessArray; //vetor de processos pais de cada processo
    int *priorityIdArray; // vetor de prioridades para cada processo (valores entre 0 e 3)
    char **processStateArray; // vetor de estados para cada processo (string com valores "Executando", "Pronto", "Bloqueado")
    Timer **initialTimerArray; // vetor de tempos de início para cada processo
    Timer **CPUTimerArray; // vetor de tempos de CPU usados para cada processo
    Ready *readyArray; //vetor de processos prontos
    Blocked *blockedArray; //vetor de processos bloqueados
    Running *executingArray;//processo em execução
} ProcessTable;

ProcessTable *initializeProcessTable(int initialCapacity);

void deleteProcessTable(ProcessTable *processTable);

void deleteProcessTableProcess(int id, ProcessTable *processTable);

void addProcessTableProcess(ProcessTable *processTable, char *filename, int parentProcess, int clock);

void copyProcess(ProcessTable *processTable, Process *proc, Timer *timer, int PcPlus);

int getProcessTableEmptySpace(ProcessTable *processTable);

int searchByIdInProcessTable(int id, ProcessTable *processTable);

void printProcessTable(ProcessTable *processTable);

int nextID(ProcessTable *processTable);

#endif