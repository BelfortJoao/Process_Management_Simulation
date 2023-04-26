#ifndef SRC_PROCESSTABLE_H
#define SRC_PROCESSTABLE_H

#include "../process/process.h"
#include "timer.h"
#include "readystate.h"
#include "blockedstate.h"
#include "executingstate.h"

typedef struct {
    int nextFreeId; // proximo idArray livre
    int tableSize; // número de processos na tabela
    int tableCapacity; // capacidade maxima da tabela
    int *emptyArray; // vetor de flags para espaços vazios
    int *idArray; //vetor de ids de processos de acordo com suas posições
    Process **processArray; // vetor de ponteiros para processos
    int *programCounterArray; // vetor de contadores de programa para cada processo
    int *parentProcessArray; //vetor de processos pais de cada processo
    int *priorityIdsArray; // vetor de prioridades para cada processo (valores entre 0 e 3)
    char **processStateArray; // vetor de estados para cada processo (string com valores "Executando", "Pronto", "Bloqueado")
    Timer *initialTimeArray; // vetor de tempos de início para cada processo
    Timer *CPUTimeArray; // vetor de tempos de CPU usados para cada processo
    Ready *readyArray; //vetor de processos prontos
    BlockedIds *blockedArray; //vetor de processos bloqueados
    currOnExecution *executingArray;//processo em execução
} ProcessTable;

void initProcessTable(ProcessTable *processTable, int initialCapacity);

void deleteProcessTable(ProcessTable *processTable);

void deleteProcessTableProcess(int ID, ProcessTable *processTable);

void addProcessTableProcess(ProcessTable *processTable, char *arq, int father, Timer clock);

void copyProcess(ProcessTable *processTable, Process *proc, Timer t, int PcPlus);

int getProcessTableEmptySpace(ProcessTable *processTable);

int searchID(int ID, ProcessTable *processTable);

void printProcessTable(ProcessTable *processTable);

int nextID(ProcessTable *processTable);

#endif