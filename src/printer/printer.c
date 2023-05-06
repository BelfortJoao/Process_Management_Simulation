#include <stdio.h>
#include <stdlib.h>
#include "printer.h"

void printMedResponseTime(Print *print) {
    printf("EU NÃO SEI O TEMPO MEDIO DE RESPOSTA POHA");
}

void printProcessTable(ProcessTable *processTable) {
    printf("Proximo idArray Livre: %d\n", processTable->nextFreeId);
    printf("Numero atual de processos: %d\n", processTable->tableSize);
    printf("Capacidade da tabela: %d\n", processTable->tableCapacity);
    printf("\n+------------+------------+------------+------------+------------+------------+------------+\n");
    printf("| %-10s | %-10s | %-10s | %-10s | %-10s | %-10s | %-10s |\n", "idArray", "PC", "Pai", "Prioridade", "Estado", "Inicio", "T de uso");
    printf("+------------+------------+------------+------------+------------+------------+------------+");
    for (int i = 0; i < processTable->tableCapacity; ++i) {
        if (processTable->emptyArray[i] != 0) {
            if (i != 0) {
            printf("\n|            |            |            |            |            |            |            |");
            }
            printf("\n| %-10d | %-10d | %-10d | %-10d | %-10s | %-10d | %-10d |", processTable->idArray[i], processTable->programCounterArray[i],
                   processTable->parentProcessArray[i], processTable->priorityIdsArray[i],
                   processTable->processStateArray[i], processTable->initialTimeArray[i],
                   processTable->CPUTimeArray[i]);
        };
    }
    printf("\n+------------+------------+------------+------------+------------+------------+------------+\n");
}

void printInitMem(char *arg1) {
    printf("Memória iniciada com tamanho %d\n", atoi(arg1));
}

void printDeclareVar(char *arg1) {
    printf("Variável %d declarada \n", atoi(arg1));
}

void printChangeVar(char *arg1, char *arg2) {
    printf("Posição de memoria %d mudada para %d\n", atoi(arg1), atoi(arg2));
}

void printAddVar(char *arg1, char *arg2) {
    printf("Posição de memória %d acrescida em %d\n", atoi(arg1), atoi(arg2));
}

void printSubVar(char *arg1, char *arg2) {
    printf("Posição de memória %d subtraida em %d\n", atoi(arg1), atoi(arg2));
}

void printBlkProcess(char *arg1) {
    printf("Processo bloqueado por %d unidades de tempo\n", atoi(arg1));
}

void printTerProcess() {
    printf("Terminando processo\n");
}

void printCopyProcess() {
    printf("Copiando processo\n");
}

void printReadFile(char *arg1) {
    printf("Lendo o arquivo %s\n", arg1);
}

void printState(Ready* rs) {
    printf("\n+-----------------+\n");
    printf("| Estado da fila: |\n");
    printf("+-----------------+\n");
    for (int i = 0; i < 4; i++) {
        printf("| Fila %d: ", i);
        if (rs->queues[i].front == NULL) {
            printf("Vazia");
        } else {
            node* atual = rs->queues[i].front;
            while (atual != NULL) {
                printf("%d ", atual->id);
                atual = atual->next;
            }
        }
        printf("\n");
    }
    printf("\n");
}
/*
void printFullQueue() {
    printf("Fila cheia\n");
}

void printProcessNotFound() {
    printf("Processo não encontrado\n");
}

void printEmptyQueue() {
    printf("Fila vazia\n");
}*/