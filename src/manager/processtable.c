#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "processtable.h"

void initProcessTable(ProcessTable *processTable, int initialCapacity) {
    processTable->tableSize = 0;
    processTable->tableCapacity = initialCapacity;
    processTable->processArray = (Process **) malloc(initialCapacity * sizeof(Process *));
    processTable->programCounterArray = (int *) malloc(initialCapacity * sizeof(int));
    processTable->idArray = (int *) malloc(initialCapacity * sizeof(int));
    processTable->parentProcessArray = (int *) malloc(initialCapacity * sizeof(int));
    processTable->priorityIdsArray = (int *) malloc(initialCapacity * sizeof(int));
    processTable->processStateArray = (char **) malloc(initialCapacity * sizeof(char *));
    processTable->initialTimeArray = (int *) malloc(initialCapacity * sizeof(int));
    processTable->CPUTimeArray = (int *) malloc(initialCapacity * sizeof(int));
    processTable->emptyArray = (int *) malloc(initialCapacity * sizeof(int));
    processTable->blockedArray = malloc(sizeof(BlockedIds));
    processTable->readyArray = malloc(sizeof(Ready));
    processTable->executingArray = malloc(sizeof(currOnExecution));
    processTable->nextFreeId = 0;
    for (int i = 0; i < initialCapacity; i++) {
        processTable->processArray[i] = (Process *) malloc(sizeof(Process));
        processTable->programCounterArray[i] = 0;
        processTable->priorityIdsArray[i] = -1;
        processTable->processStateArray[i] = "BLOQUEADO";
        processTable->emptyArray[i] = 0;
        initTime(&processTable->initialTimeArray[i]);
        initTime(&processTable->CPUTimeArray[i]);
        processTable->parentProcessArray[i] = -1;
    }
    contextExchange(-1, processTable->executingArray);
    initBlockedIds(processTable->blockedArray, initialCapacity);
    initReady(processTable->readyArray, initialCapacity);
}

int getProcessTableEmptySpace(ProcessTable *processTable) {
    for (int i = 0; i < processTable->tableCapacity; ++i) {
        if (processTable->emptyArray[i] == 0) {
            return i;
        }
    }
    return -1;
}

void addProcessTableProcess(ProcessTable *processTable, char *arq, int father, Timer clock) {
    processTable->tableSize++;
    int i = getProcessTableEmptySpace(processTable);
    processTable->emptyArray[i] = 1;
    initProcess(processTable->processArray[i], arq);
    processTable->priorityIdsArray[i] = 0;
    processTable->processStateArray[i] = strdup("PRONTO");
    processTable->parentProcessArray[i] = father;
    processTable->initialTimeArray[i] = clock;
    processTable->CPUTimeArray[i] = (Timer) 0;
    processTable->idArray[i] = nextID(processTable);
    insertReady(processTable->readyArray, i, processTable->priorityIdsArray[i]);
}

void deleteProcessTableProcess(int ID, ProcessTable *processTable) {
    int i = searchID(ID, processTable);
    excludeProcess(processTable->processArray[i]);
    removeReady(processTable->readyArray, i);
    processTable->tableSize--;
    processTable->emptyArray[i] = 0;
    processTable->processArray = NULL;
    processTable->priorityIdsArray[i] = -1;
    processTable->programCounterArray[ID] = 0;
    processTable->processStateArray[ID] = "BLOQUEADO";
    processTable->parentProcessArray[ID] = -1;
    processTable->initialTimeArray[ID] = -1;
    processTable->CPUTimeArray[ID] = -1;
    removeBlockedId(processTable->blockedArray, ID);

}

void upPC(ProcessTable *pt) {
    pt->programCounterArray++;
}

void deleteProcessTable(ProcessTable *processTable) {
    for (int i = 0; i < processTable->tableCapacity; ++i) {
        if (processTable->processArray[i] != NULL) {
            excludeProcess(processTable->processArray[i]);
        }
    }
    freeReady(processTable->readyArray);
    freeBlockedIds(processTable->blockedArray);
    free(processTable->processArray);
    free(processTable->emptyArray);
    free(processTable->programCounterArray);
    free(processTable->parentProcessArray);
    free(processTable->priorityIdsArray);
    free(processTable->processStateArray);
    free(processTable->initialTimeArray);
    free(processTable->CPUTimeArray);
    free(processTable);
}

int searchID(int ID, ProcessTable *processTable) {
    for (int i = 0; i < processTable->tableCapacity; ++i) {
        if (processTable->idArray[i] == ID) {
            return i;
        }
    }
    return -1;
}

void printProcessTable(ProcessTable *processTable) {
    printf("Proximo idArray Livre: %d\n", processTable->nextFreeId);
    printf("Numero atual de processos: %d\n", processTable->tableSize);
    printf("Capacidade da tabela: %d\n", processTable->tableCapacity);
    printf("| %-10s | %-10s | %-10s | %-10s | %-10s | %-10s | %-10s |\n", "idArray", "PC", "Pai", "Prioridade", "Estado", "Inicio", "T de uso");
    printf("============================================================================================");
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
    printf("\n");
}

int nextID(ProcessTable *processTable) {
    processTable->nextFreeId++;
    return (processTable->nextFreeId - 1);
}

void copyProcess(ProcessTable *processTable, Process *proc, Timer t, int PcPlus) {
    processTable->tableSize++;
    int i = getProcessTableEmptySpace(processTable);
    processTable->emptyArray[i] = 1;
    processTable->priorityIdsArray[i] = 0;
    processTable->programCounterArray[i] = processTable->programCounterArray[*processTable->executingArray] + PcPlus;
    processTable->processStateArray[i] = "PRONTO";
    processTable->parentProcessArray[i] = *processTable->executingArray;
    processTable->initialTimeArray[i] = t;
    processTable->CPUTimeArray[i] = 0;
    processTable->idArray[i] = nextID(processTable);
    insertReady(processTable->readyArray, i, processTable->priorityIdsArray[i]);
}

void rewid(ProcessTable *processTable) {

}
