//
// Created by belfort on 4/22/23.
//

#include "ProcessTable.h"
void inicializarTabelaDeProcessos(processTable* table, int initialCapacity) {

    table->numProcess = 0;
    table->tableCapacity = initialCapacity;
    table->proc = (struct process**) malloc(initialCapacity * sizeof(struct process*));
    table->pc = (int*) malloc(initialCapacity * sizeof(int));
    table->father = (int*) malloc(initialCapacity * sizeof(int));
    table->prioritis = (int*) malloc(initialCapacity * sizeof(int));
    table->states = (char**) malloc(initialCapacity * sizeof(char*));
    table->initialTime = (int**) malloc(initialCapacity * sizeof(int));
    table->CPUTime = (int**) malloc(initialCapacity * sizeof(int));
    table->ampitySpace=(int*) malloc(initialCapacity * sizeof(int));

    for (int i = 0; i < initialCapacity; i++) {
        table->proc[i] = NULL;
        table->pc[i] = 0;
        table->prioritis[i] = -1;
        table->states[i] = strdup("Bloqueado");
        table->ampitySpace=0;
        *table->initialTime[i] = -1;
        *table->CPUTime[i] = -1;
        table->father[i] = -1;
    }
    contextExchange(-1, table->ex);
    initBlockeds(table->bk, initialCapacity);
    initReady(table->rd,initialCapacity);
}
int searchampitySpaceInProcessTable(processTable* pt){
    for (int i = 0; i < pt->tableCapacity; ++i) {
        if(pt->ampitySpace[i]==0){
            return i;
        }
    }
    return -1;
}
void addProcess(processTable* pt, char* arq, int prior, int father){
    process *proc;
    pt->numProcess++;
    int ID=searchampitySpaceInProcessTable(pt);
    pt->ampitySpace[ID]=1;
    initProcess(proc, arq);
    pt->proc[ID]=proc;
    pt->prioritis[ID]=prior;
    pt->states[ID]=strdup("Pronto");
    pt->father[ID]=father;
//    pt->initialTime[ID]=; Tempo inicial do processo
    pt->CPUTime[ID]=0;
    insertReady(pt->rd,ID,prior);
}
void excludeProcessInPT(int ID, processTable* pt){
    excludeProcess(pt->proc[ID]);
    removeReady(pt->rd,ID, pt->prioritis[ID]);
    pt->numProcess--;
    pt->ampitySpace[ID]=0;
    pt->proc=NULL;
    pt->prioritis[ID]=-1;
    pt->pc[ID]=0;
    pt->states[ID]=strdup("Bloqueado");
    pt->father[ID]=-1;
    *pt->initialTime[ID]= -1;
    *pt->CPUTime[ID]=-1;
    removeBlocked(*pt->bk,ID);

}
void changeState(processTable* pt, int ID, char* state){
    pt->states[ID]=state;
}

void upPC(processTable* pt, int ID){
    pt->pc++;
}

void excludeProcessTable(processTable* pt){
    for (int i = 0; i < pt->tableCapacity; ++i) {
        if(pt->proc[i]!=NULL){
            excludeProcess(pt->proc[i]);
        }
    }
    freeReady(pt->rd);
    freeBlockeds(pt->bk);
    free(pt->proc);
    free(pt->ampitySpace);
    free(pt->pc);
    free(pt->father);
    free(pt->prioritis);
    free(pt->states);
    free(pt->initialTime);
    free(pt->CPUTime);
    free(pt);
}
