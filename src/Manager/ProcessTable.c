//
// Created by belfort on 4/22/23.
//

#include "ProcessTable.h"
void inicializarTabelaDeProcessos(processTable* table, int initialCapacity) {
    table->numProcess = 0;
    table->tableCapacity = initialCapacity;
    table->proc = (struct process**) malloc(initialCapacity * sizeof(struct process*));
    table->pc = (int*) malloc(initialCapacity * sizeof(int));
    table->ID = (int*) malloc(initialCapacity * sizeof(int));
    table->father = (int*) malloc(initialCapacity * sizeof(int));
    table->prioritis = (int*) malloc(initialCapacity * sizeof(int));
    table->states = (char**) malloc(initialCapacity * sizeof(char*));
    table->initialTime = (int*) malloc(initialCapacity * sizeof(int));
    table->CPUTime = (int*) malloc(initialCapacity * sizeof(int));
    table->ampitySpace=(int*) malloc(initialCapacity * sizeof(int));
    table->bk= malloc(sizeof (blockeds));
    table->rd= malloc(sizeof (Ready));
    table->ex= malloc(sizeof(executing));
    table->idLv=0;
    for (int i = 0; i < initialCapacity; i++) {
        table->proc[i] = (struct process*) malloc( sizeof(struct process));
        table->pc[i] = 0;
        table->prioritis[i] = -1;
        table->states[i] = "BLOQUEADO";
        table->ampitySpace[i]=0;
        initTime(&table->initialTime[i]);
        initTime(&table->CPUTime[i]);
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
void addProcess(processTable* pt, char* arq, int father, timer clock){
    pt->numProcess++;
    int i=searchampitySpaceInProcessTable(pt);
    printf("%d",i);
    pt->ampitySpace[i]=1;
    initProcess(pt->proc[i], arq);
    pt->prioritis[i]=0;
    pt->states[i]=strdup("PRONTO");
    pt->father[i]=father;
    pt->initialTime[i]= clock;
    pt->CPUTime[i]=(timer)0;
    pt->ID[i]= nextID(pt);
    insertReady(pt->rd,i);
}
void excludeProcessInPT(int ID, processTable* pt){
    int i=searchID(ID,pt);
    excludeProcess(pt->proc[i]);
    removeReady(pt->rd,i);
    pt->numProcess--;
    pt->ampitySpace[i]=0;
    pt->proc=NULL;
    pt->prioritis[i]=-1;
    pt->pc[ID]=0;
    pt->states[ID]="BLOQUEADO";
    pt->father[ID]=-1;
    pt->initialTime[ID]= -1;
    pt->CPUTime[ID]=-1;
    removeBlocked(pt->bk,ID);

}

void upPC(processTable* pt){
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
int searchID(int ID, processTable* pt){
    for (int i = 0; i < pt->tableCapacity; ++i) {
        if(pt->ID[i]==ID){
            return i;
        }
    }
    return -1;
}

void printProcessTable(processTable* pt) {
    printf("Proximo ID Livre: %d\n", pt->idLv);
    printf("Numero atual de processos: %d\n", pt->numProcess);
    printf("Capacidade da tabela: %d\n", pt->tableCapacity);
    printf(":ID:\t:PC:\t:Pai:\t:Prioridade:\t:Estado:\t:Inicio:\t:T de uso:");
    for (int i = 0; i < pt->tableCapacity; ++i) {
        if(pt->ampitySpace[i]!=0){
            printf("\n%d\t%d\t %d\t%d\t%s\t%d\t%d\n",pt->ID[i],pt->pc[i],pt->father[i],pt->prioritis[i],pt->states[i],pt->initialTime[i],pt->CPUTime[i]);
        };
    }
}
int nextID(processTable* pt){
    pt->idLv++;
    return (pt->idLv-1);
}

void copyProcess(processTable* pt,process* proc,timer t, int PcPlus){
    pt->numProcess++;
    int i=searchampitySpaceInProcessTable(pt);
    pt->ampitySpace[i]=1;
    pt->proc[i]->prog=proc->prog;
    pt->prioritis[i]=0;
    pt->pc[i]=pt->pc[*pt->ex]+PcPlus;
    pt->states[i]=strdup("PRONTO");
    pt->father[i]=*pt->ex;
    pt->initialTime[i]= t;
    pt->CPUTime[i]=0;
    pt->ID[i]= nextID(pt);
    insertReady(pt->rd,i);
    return;
}
void rewid(processTable* pt){

}
