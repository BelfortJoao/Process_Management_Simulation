#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include "manager.h"

void initComputer(Computer *comp, char *arq) {

    int tam = 0;

    initTime(&comp->timer);
    initCPU(&comp->cpu, arq);

    while (tam <= 0) {
        printf("Type the Process Table Capacity: ");
        scanf("%d", &tam);
    }

    initProcessTable(&(comp->processTable), tam);
    printf("Process Table Capacity: %d\n", comp->processTable.tableCapacity);

    addProcessTableProcess(&comp->processTable, arq, -1, 0);
    removeReady(comp->processTable.readyArray, 0);
    contextExchange(0, comp->processTable.executingArray);
    comp->processTable.processStateArray[0]="EXECUTANDO";
    comp->timer = 0;
    comp->kill = 0;
    comp->freeID = 0;
}

//(B n)Bloqueia o processo na CPU em N unidades de tempo
void blockProcess(Computer *comp, int blockT) {
    int go_exec;
    int go_block;
    if (comp->processTable.executingArray == 0 || comp->processTable.executingArray == NULL) {
        return;
    }
    //Operação em tabela-sai de readyArray e vai para executando
    go_block = *comp->processTable.executingArray;
    go_exec = nextReady(comp->processTable.readyArray);
    contextExchange(go_exec, comp->processTable.executingArray);
    removeReady(comp->processTable.readyArray, go_exec);
    //Operação em tabela-sai de executando e vai para block
    insertBlockedId(comp->processTable.blockedArray, go_block, blockT);
    //Operação real
    int i = searchID(go_exec, &comp->processTable);
    changeProcess(&comp->cpu, comp->processTable.processArray[i], comp->processTable.programCounterArray[i],
                  *comp->processTable.CPUTimeArray, 0);
    comp->processTable.processStateArray[go_exec] = "EXECUTANDO";
    comp->processTable.processStateArray[go_block] = "BLOQUEADO";
}

void scheduleProcess(Computer *comp) {
    int go_exec;
    int go_ready;
    int time = 1;
    //Operação em tabela-sai de readyArray e vai para executando
    go_ready = *comp->processTable.executingArray;
    go_exec = nextReady(comp->processTable.readyArray);
    if (go_exec == -1) {
        return;
    }
    comp->processTable.priorityIdsArray[*comp->processTable.executingArray]++;
    contextExchange(go_exec, comp->processTable.executingArray);
    removeReady(comp->processTable.readyArray, go_exec);
    //Operação em tabela-sai de executando e vai para pronto
    int i = searchID(go_exec, &comp->processTable);
    int j= searchID(go_ready, &comp->processTable);
    printf("ID ARRAY: %d\n", comp->processTable.idArray[i]);
    printf("PRIOR ID: %d\n", comp->processTable.priorityIdsArray[j]);
    printf("EU TO AQUIIIIIIIIIIIIIIIIII\n");
    printProcessTable(&comp->processTable);
    insertReady(comp->processTable.readyArray, go_ready,
                comp->processTable.priorityIdsArray[j]);
    //sortReady(comp->processTable.readyArray);

    printProcessTable(&comp->processTable);
    //Operação real
    if (pow(2,(4 - comp->processTable.priorityIdsArray[i]) -1) >= 1) {
        time = (int) pow(2, (4 - comp->processTable.priorityIdsArray[i]) - 1);
    }
    printf("\n\n\n\n%d\n\n\n\n", time);
    changeProcess(&comp->cpu, comp->processTable.processArray[i], comp->processTable.programCounterArray[i], time, 0);
    comp->processTable.processStateArray[go_exec] = "EXECUTANDO";
    comp->processTable.processStateArray[go_ready] = "PRONTO";
    printProcessTable(&comp->processTable);
}

//T termina o processo simulado atual e passa o cpu para o proximo processo pronto
void endProcess(Computer *comp) {
    int go_excl;
    int go_exec;
    if (comp->processTable.executingArray <= 0 || comp->processTable.executingArray == NULL) {
        return;
    }
    //Operação em tabela-sai de readyArray e vai para executando
    go_excl = *comp->processTable.executingArray;
    go_exec = nextReady(comp->processTable.readyArray);
    contextExchange(go_exec, comp->processTable.executingArray);
    removeReady(comp->processTable.readyArray, go_exec);
    //Sai de Ready e vai para executingArray

    //Operação real
    int i = searchID(go_exec, &comp->processTable);
    if(i!=-1){
        changeProcess(&comp->cpu, comp->processTable.processArray[i], comp->processTable.programCounterArray[i],
                      *comp->processTable.CPUTimeArray, 0);
        comp->processTable.processStateArray[go_exec] = "EXECUTANDO";
    }
    //exclui processo da tabela de processos
    comp->processTable.executingArray=NULL;
    comp->cpu.proc=NULL;
    deleteProcessTableProcess(go_excl, &comp->processTable);
};

void killComputer(Computer *comp) {
    freeCPU(&comp->cpu);
    deleteProcessTable(&comp->processTable);
    comp->timer = 0;
    comp->kill = 1;
    comp->freeID = 0;
}

void execute(Computer *comp) {
    int go_exec;
    //Operação sob a tabela Ready
    go_exec = nextReady(comp->processTable.readyArray);
    if(go_exec==-1 || comp->processTable.executingArray==NULL){
        printf("There is nothing more to execute\n");
        return;
    }
    contextExchange(go_exec, comp->processTable.executingArray);
    removeReady(comp->processTable.readyArray, go_exec);
    //Operação Real
    int i = searchID(go_exec, &comp->processTable);
    changeProcess(&comp->cpu, comp->processTable.processArray[i], comp->processTable.programCounterArray[i],
                  *comp->processTable.CPUTimeArray, 0);
    comp->processTable.processStateArray[go_exec] = "EXECUTANDO";
}

void processExecuting(Computer *comp) {
    //search for a proces while cpu is ampity
    if (comp->processTable.executingArray < 0 || comp->processTable.executingArray == NULL || comp->cpu.proc==NULL) {
        //terminate the Computer if kill switch is equal one
        execute(comp);
        if (comp->kill == 1) {
            killComputer(comp);
            return;
        }
        //if cpu isn't ampity check the cpu time and escalonate
    } else {
        printf("\n\n\n\n%d\n\n\n\n", comp->cpu.executing_timer);
        printf("\n\n\n\n%d\n\n\n\n", comp->cpu.program_timer);
        if (comp->cpu.executing_timer >= comp->cpu.program_timer) {
            scheduleProcess(comp);
        }

        //Interpreta o processo aumenta o timer
        uperInterpreter(comp);
        clockUpPC(comp);
        processUnblock(comp);
        printState(comp->processTable.readyArray);
        //Check the kill switch
        if (comp->kill == 1) {
            killComputer(comp);
            return;
        }
    }
}

void processUnblock(Computer *comp) {
    int go_ready;
    for (int i = 0; i < comp->processTable.tableCapacity; ++i) {
        if (comp->processTable.blockedArray->blockTime[i] == 0) {
            go_ready = comp->processTable.blockedArray->id[i];
            removeBlockedId(comp->processTable.blockedArray, go_ready);
            insertReady(comp->processTable.readyArray, go_ready,
                        comp->processTable.priorityIdsArray[searchID(go_ready, &comp->processTable)]);
            comp->processTable.processStateArray[searchID(go_ready, &comp->processTable)] = "PRONTO";
        }
    }
}

void clockUpPC(Computer *comp) {
    timeUp(&comp->timer);
    timeUp(&comp->cpu.executing_timer);
    blockDownClock(comp->processTable.blockedArray);
}




void processCP(Computer *comp, Process *proc, int PcPlus) {

    proc = generateNewProcess(comp->cpu.proc);
    copyProcess(&comp->processTable, proc, comp->timer, PcPlus);
}

void processRewind(Computer *comp, char *arq) {
    excludeProcess(comp->processTable.processArray[*comp->processTable.executingArray]);
    initProcess(comp->processTable.processArray[*comp->processTable.executingArray], arq);
}

void attExec(Computer *comp) {
    comp->processTable.programCounterArray[*comp->processTable.executingArray] = comp->cpu.pc;
    comp->processTable.processArray[*comp->processTable.executingArray] = comp->cpu.proc;
    comp->processTable.CPUTimeArray[*comp->processTable.executingArray]++;
}

void uperInterpreter(Computer *comp) {
    int blk;
    Process *proc;
    int PcPlus;
    char **arq;
    printf("PC: %d\n", comp->cpu.pc);
    int cpuResp = interpreter(&comp->cpu, &blk, arq, &PcPlus);
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
            processCP(comp, proc, PcPlus);
            return;
        case 4://Recria o processo atual com base em um arquivo
            processRewind(comp, *arq);
            return;
    }
}