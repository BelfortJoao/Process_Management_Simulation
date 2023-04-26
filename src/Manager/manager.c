//
// Created by belfort on 4/23/23.
//
#include <math.h>
#include "manager.h"
//Loop infinito

void initComputer(computer* comp, char* arq){
    int tam=0;
    initTime(&comp->clock);
    initCPU(&comp->cpu, arq);
    while(tam<=0) {
        printf("Type the process Table Capacity: ");
        scanf("%d", &tam);
    }
    inicializarTabelaDeProcessos(&comp->proctb,tam);
    addProcess(&comp->proctb,arq,-1,0);
    removeReady(comp->proctb.rd,0);
    contextExchange(0,comp->proctb.ex);
    comp->clock=0;
    comp->kill=0;
    comp->freeID=0;
}

//(B n)Bloqueia o processo na CPU em N unidades de tempo
void processBlock(computer* comp, int blockT){
    int go_exec;
    int go_block;
    if(comp->proctb.ex == 0 || comp->proctb.ex == NULL){
        return;
    }
    //Operação em tabela-sai de ready e vai para executando
    go_block=*comp->proctb.ex;
    go_exec=nextReady(comp->proctb.rd);
    contextExchange(go_exec,comp->proctb.ex);
    removeReady(comp->proctb.rd,go_exec);
    //Operação em tabela-sai de executando e vai para block
    insertBlocked(comp->proctb.bk, go_block, blockT);
    //Operação real
    int i=searchID(go_exec,&comp->proctb);
    changeProcess(&comp->cpu,comp->proctb.proc[i],comp->proctb.pc[i],*comp->proctb.CPUTime,0);
    comp->proctb.states[go_exec]="EXECUTANDO";
    comp->proctb.states[go_block]="BLOQUEADO";
}
void processEscalonating(computer* comp){
    int go_exec;
    int go_ready;
    int time=2;
    //Operação em tabela-sai de ready e vai para executando
    go_ready=*comp->proctb.ex;
    go_exec=nextReady(comp->proctb.rd);
    if(go_exec==-1){
        return;
    }
    comp->proctb.prioritis[*comp->proctb.ex]++;
    contextExchange(go_exec,comp->proctb.ex);
    removeReady(comp->proctb.rd,go_exec);
    //Operação em tabela-sai de executando e vai para pronto
    insertReady(comp->proctb.rd,go_ready,comp->proctb.prioritis[*comp->proctb.ex]);
    //Operação real
    int i=searchID(go_exec,&comp->proctb);
    if(2*(4-*comp->proctb.prioritis)>=2){
        time =(int)pow(2,(4-*comp->proctb.prioritis)-1);
    }
    printf("\n\n\n\n%d\n\n\n\n",time);
    changeProcess(&comp->cpu,comp->proctb.proc[i],comp->proctb.pc[i],time,0);
    comp->proctb.states[go_exec]="EXECUTANDO";
    comp->proctb.states[go_ready]="PRONTO";
}

//T termina o processo simulado atual e passa o cpu para o proximo processo pronto
void processExterminate(computer* comp){
    int go_excl;
    int go_exec;
    if(comp->proctb.ex <= 0 || comp->proctb.ex == NULL){
        return;
    }
    //Operação em tabela-sai de ready e vai para executando
    go_excl=*comp->proctb.ex;
    go_exec=nextReady(comp->proctb.rd);
    contextExchange(go_exec,comp->proctb.ex);
    removeReady(comp->proctb.rd,go_exec);
    //Sai de Ready e vai para ex

    //Operação real
    int i=searchID(go_exec,&comp->proctb);
    changeProcess(&comp->cpu,comp->proctb.proc[i],comp->proctb.pc[i],*comp->proctb.CPUTime,0);
    comp->proctb.states[go_exec]="EXECUTANDO";
    //exclui processo da tabela de processos
    excludeProcessInPT(go_excl,&comp->proctb);
};

void computerKill(computer* comp){
    freeCPU(&comp->cpu);
    excludeProcessTable(&comp->proctb);
    comp->clock=0;
    comp->kill=1;
    comp->freeID=0;
}
void execute(computer* comp){
    int go_exec;
    //Operação sob a tabela Ready
    go_exec=nextReady(comp->proctb.rd);
    contextExchange(go_exec,comp->proctb.ex);
    removeReady(comp->proctb.rd,go_exec);
    //Operação Real
    int i=searchID(go_exec,&comp->proctb);
    changeProcess(&comp->cpu,comp->proctb.proc[i],comp->proctb.pc[i],*comp->proctb.CPUTime,0);
    comp->proctb.states[go_exec]="EXECUTANDO";
}
void processExecuting(computer* comp){
        //search for a proces while cpu is ampity
        if (comp->proctb.ex <=0||comp->proctb.ex == NULL||comp->cpu.proc->numLines==0) {
                //terminate the computer if kill switch is equal one
                if (comp->kill==1){
                    computerKill(comp);
                    return;
                }
            //if cpu isn't ampity check the cpu time and escalonate
        } else {
            if(comp->cpu.executing_timer>=comp->cpu.program_timer){
                processEscalonating(comp);
            }
        }
        //Interpreta o processo aumenta o clock
        uperInterpreter(comp);
        clockUpPC(comp);
        processUnblock(comp);
        //Check the kill switch
        if (comp->kill==1){
            computerKill(comp);
            return;
        }
}

void processUnblock(computer* comp){
    int go_ready;
    for (int i = 0; i < comp->proctb.tableCapacity; ++i) {
        if(comp->proctb.bk->blocktime[i]==0){
            go_ready=comp->proctb.bk->Id[i];
            removeBlocked(comp->proctb.bk,go_ready);
            insertReady(comp->proctb.rd,go_ready,comp->proctb.prioritis[searchID(go_ready,&comp->proctb)]);
            comp->proctb.states[searchID(go_ready,&comp->proctb)]="PRONTO";
        }
    }
}

void clockUpPC(computer* comp){
    timeUp(&comp->clock);
    timeUp(&comp->cpu.executing_timer);
    blockDownclock(comp->proctb.bk);
}

void processCP(computer* comp, process* proc,int PcPlus){
    proc= (struct process*) malloc( sizeof(struct process));
    proc->numLines=comp->cpu.proc->numLines;
    proc->lengthMem=comp->cpu.proc->lengthMem;
    if (proc->prog ==NULL){
        proc->prog = (char**) malloc(proc->numLines * sizeof(char*));
    }
    for (int i = 0; i < proc->numLines; i++) {
        if(proc->prog[i]==NULL){proc->prog[i] = (char*) malloc(CHAR_MAX * sizeof(char));
            }
        proc->prog[i]=comp->cpu.proc->prog[i];
    }
    for (int i = 0; i < proc->lengthMem; i++) {
        proc->mem[i]=comp->cpu.proc->mem[i];
    }
    copyProcess(&comp->proctb,proc, comp->clock,PcPlus);
}
void processRewind(computer* comp, char* arq){
    excludeProcess(comp->proctb.proc[*comp->proctb.ex]);
    initProcess(comp->proctb.proc[*comp->proctb.ex], arq);
}
void attExec(computer* comp){
    comp->proctb.pc[*comp->proctb.ex]=comp->cpu.pc;
    comp->proctb.proc[*comp->proctb.ex]=comp->cpu.proc;
    comp->proctb.CPUTime[*comp->proctb.ex]++;
}

void uperInterpreter(computer* comp){
    int blk;
    int PcPlus;
    process* proc;
    char** arq;
    int cpuResp= interpreter(&comp->cpu, &blk,proc, arq,&PcPlus);
    comp->cpu.pc++;
    attExec(comp);
    switch (cpuResp) {
        case 0:
            return;
        case 1://Bloqueia esse processo simulado por n unidades de tempo.
            processBlock(comp,blk);
            return;
        case 2://Acaba com o processo atual e coloca outro no lugar
            processExterminate(comp);
            return;
        case 3://Crian um novo processo com base no atual do CPU
            processCP(comp,proc,PcPlus);
            return;
        case 4://Recria o processo atual com base em um arquivo
            processRewind(comp, *arq);
            return;
    }
}