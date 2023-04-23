//
// Created by belfort on 4/22/23.
//

#ifndef SRC_PROCESSTABLE_H
#define SRC_PROCESSTABLE_H

#include "../Process.h"
#include "timer.h"

typedef struct processTable {
    int numProcess; // número de processos na tabela
    int tableCapacity;
    int* ampitySpace;
    process** proc; // vetor de ponteiros para processos
    int* pc; // vetor de contadores de programa para cada processo
    int* father; //vetor de processos pais de cada processo
    int* prioritis; // vetor de prioridades para cada processo (valores entre 0 e 3)
    char** states; // vetor de estados para cada processo (string com valores "Executando", "Pronto", "Bloqueado")
    timer* initialTime; // vetor de tempos de início para cada processo
    timer* CPUTime; // vetor de tempos de CPU usados para cada processo
}processTable;

void excludeProcessTable(processTable* pt);
void changeState(processTable* pt, int ID, char* state);
void excludeProcessInPT(int ID, processTable* pt);
void addProcess(processTable* pt, char* arq, int prior, int father);
int searchampitySpaceInProcessTable(processTable* pt);
void inicializarTabelaDeProcessos(processTable* table, int initialCapacity);
#endif //SRC_PROCESSTABLE_H
