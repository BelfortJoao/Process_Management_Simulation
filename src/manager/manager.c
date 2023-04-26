#include <limits.h>
#include <math.h>

#include "manager.h"

void initComputer(Computer* comp, char* arq){
    int tam=0;
    initTime(&comp->timer);
    initCPU(&comp->cpu, arq);
    while(tam<=0) {
        printf("Type the Process Table Capacity: ");
        scanf("%d", &tam);
    }
    initProcessTable(&comp->processTable, tam);
    addProcessTableProcess(&comp->processTable, arq, -1, 0);
    removeReady(comp->processTable.readyArray, 0);
    contextExchange(0,comp->processTable.executingArray);
    comp->timer=0;
    comp->kill=0;
    comp->freeID=0;
}

//(B n)Bloqueia o processo na CPU em N unidades de tempo
void blockProcess(Computer* comp, int blockT){
    int go_exec;
    int go_block;
    if(comp->processTable.executingArray == 0 || comp->processTable.executingArray == NULL){
        return;
    }
    //Operação em tabela-sai de readyArray e vai para executando
    go_block=*comp->processTable.executingArray;
    go_exec=nextReady(comp->processTable.readyArray);
    contextExchange(go_exec,comp->processTable.executingArray);
    removeReady(comp->processTable.readyArray, go_exec);
    //Operação em tabela-sai de executando e vai para block
    insertBlockedId(comp->processTable.bk, go_block, blockT);
    //Operação real
    int i=searchID(go_exec,&comp->processTable);
    changeProcess(&comp->cpu, comp->processTable.processArray[i], comp->processTable.programCounterArray[i], *comp->processTable.CPUTimeArray, 0);
    comp->processTable.processStateArray[go_exec]="EXECUTANDO";
    comp->processTable.processStateArray[go_block]="BLOQUEADO";
}
void scheduleProcess(Computer* comp){
    int go_exec;
    int go_ready;
    int time=2;
    //Operação em tabela-sai de readyArray e vai para executando
    go_ready=*comp->processTable.executingArray;
    go_exec=nextReady(comp->processTable.readyArray);
    if(go_exec==-1){
        return;
    }
    comp->processTable.priorityIdsArray[*comp->processTable.executingArray]++;
    contextExchange(go_exec,comp->processTable.executingArray);
    removeReady(comp->processTable.readyArray, go_exec);
    //Operação em tabela-sai de executando e vai para pronto
    insertReady(comp->processTable.readyArray, go_ready, comp->processTable.priorityIdsArray[*comp->processTable.executingArray]);
    //Operação real
    int i=searchID(go_exec,&comp->processTable);
    if(2*(4-*comp->processTable.priorityIdsArray) >= 2){
        time =(int)pow(2, (4-*comp->processTable.priorityIdsArray) - 1);
    }
    printf("\n\n\n\n%d\n\n\n\n",time);
    changeProcess(&comp->cpu, comp->processTable.processArray[i], comp->processTable.programCounterArray[i], time, 0);
    comp->processTable.processStateArray[go_exec]="EXECUTANDO";
    comp->processTable.processStateArray[go_ready]="PRONTO";
}

//T termina o processo simulado atual e passa o cpu para o proximo processo pronto
void endProcess(Computer* comp){
    int go_excl;
    int go_exec;
    if(comp->processTable.executingArray <= 0 || comp->processTable.executingArray == NULL){
        return;
    }
    //Operação em tabela-sai de readyArray e vai para executando
    go_excl=*comp->processTable.executingArray;
    go_exec=nextReady(comp->processTable.readyArray);
    contextExchange(go_exec,comp->processTable.executingArray);
    removeReady(comp->processTable.readyArray, go_exec);
    //Sai de Ready e vai para executingArray

    //Operação real
    int i=searchID(go_exec,&comp->processTable);
    changeProcess(&comp->cpu, comp->processTable.processArray[i], comp->processTable.programCounterArray[i], *comp->processTable.CPUTimeArray, 0);
    comp->processTable.processStateArray[go_exec]="EXECUTANDO";
    //exclui processo da tabela de processos
    deleteProcessTableProcess(go_excl, &comp->processTable);
};

void killComputer(Computer* comp){
    freeCPU(&comp->cpu);
    deleteProcessTable(&comp->processTable);
    comp->timer=0;
    comp->kill=1;
    comp->freeID=0;
}

void execute(Computer* comp){
    int go_exec;
    //Operação sob a tabela Ready
    go_exec=nextReady(comp->processTable.readyArray);
    contextExchange(go_exec,comp->processTable.executingArray);
    removeReady(comp->processTable.readyArray, go_exec);
    //Operação Real
    int i=searchID(go_exec,&comp->processTable);
    changeProcess(&comp->cpu, comp->processTable.processArray[i], comp->processTable.programCounterArray[i], *comp->processTable.CPUTimeArray, 0);
    comp->processTable.processStateArray[go_exec]="EXECUTANDO";
}
void processExecuting(Computer* comp){
        //search for a proces while cpu is ampity
        if (comp->processTable.executingArray <= 0 || comp->processTable.executingArray == NULL || comp->cpu.proc->numLines == 0) {
                //terminate the Computer if kill switch is equal one
                if (comp->kill==1){
                    killComputer(comp);
                    return;
                }
            //if cpu isn't ampity check the cpu time and escalonate
        } else {
            if(comp->cpu.executing_timer>=comp->cpu.program_timer){
                scheduleProcess(comp);
            }
        }
        //Interpreta o processo aumenta o timer
        uperInterpreter(comp);
        clockUpPC(comp);
        processUnblock(comp);
        //Check the kill switch
        if (comp->kill==1){
            killComputer(comp);
            return;
        }
}

void processUnblock(Computer* comp){
    int go_ready;
    for (int i = 0; i < comp->processTable.tableCapacity; ++i) {
        if(comp->processTable.bk->blockTime[i] == 0){
            go_ready=comp->processTable.bk->idArray[i];
            removeBlockedId(comp->processTable.bk, go_ready);
            insertReady(comp->processTable.readyArray, go_ready, comp->processTable.priorityIdsArray[searchID(go_ready, &comp->processTable)]);
            comp->processTable.processStateArray[searchID(go_ready, &comp->processTable)]="PRONTO";
        }
    }
}

void clockUpPC(Computer* comp){
    timeUp(&comp->timer);
    timeUp(&comp->cpu.executing_timer);
    blockDownClock(comp->processTable.bk);
}

void processCP(Computer* comp, Process* proc, int PcPlus){
    proc= (struct Process*) malloc(sizeof(struct Process));
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
    copyProcess(&comp->processTable, proc, comp->timer, PcPlus);
}
void processRewind(Computer* comp, char* arq){
    excludeProcess(comp->processTable.processArray[*comp->processTable.executingArray]);
    initProcess(comp->processTable.processArray[*comp->processTable.executingArray], arq);
}
void attExec(Computer* comp){
    comp->processTable.programCounterArray[*comp->processTable.executingArray]=comp->cpu.pc;
    comp->processTable.processArray[*comp->processTable.executingArray]=comp->cpu.proc;
    comp->processTable.CPUTimeArray[*comp->processTable.executingArray]++;
}

void uperInterpreter(Computer* comp){
    int blk;
    int PcPlus;
    Process* proc;
    char** arq;
    int cpuResp= interpreter(&comp->cpu, &blk,proc, arq,&PcPlus);
    comp->cpu.pc++;
    attExec(comp);
    switch (cpuResp) {
        case 0:
            return;
        case 1://Bloqueia esse processo simulado por n unidades de tempo.
            blockProcess(comp, blk);
            return;
        case 2://Acaba com o processo atual e coloca outro no lugar
            endProcess(comp);
            return;
        case 3://Crian um novo processo com base no atual do CPU
            processCP(comp,proc,PcPlus);
            return;
        case 4://Recria o processo atual com base em um arquivo
            processRewind(comp, *arq);
            return;
    }
}