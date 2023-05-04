#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../input/input.h"
#include "../error/error.h"

#include "processmanager.h"


ProcessManager *initializeProcessManager()
{
    ProcessManager *processManager = (ProcessManager *) malloc(sizeof(ProcessManager));

    if (!processManager)
    {
        printf(ALLOCATION_ERROR, "process manager");
        return NULL;
    }

    return processManager;
}

ProcessManager *initializeProcessManagerFromFile(char *filename)
{
    ProcessManager *processManager = initializeProcessManager();

    if (!processManager)
    {
        return NULL;
    }

    int size = 0;

    processManager->timer = initializeTimer();

    if (!processManager->timer)
    {
        return NULL;
    }

    processManager->cpu = initializeCPU(filename);

    if (!processManager->cpu)
    {
        return NULL;
    }

    while (size <= 0)
    {
        printf("\nType the Process Table Capacity: ");
        if (!scanf("%d", &size))
        {
            printf("\nInvalid capacity for the table->\n");
            cleanStdin();
        }
    }

    processManager->processTable = initializeProcessTable(size);
    printf("Process Table Capacity: %d\n", processManager->processTable->tableCapacity);

    addProcessTableProcess(processManager->processTable, filename, -1, 0);
    removeFromReadyQueue(processManager->processTable->readyArray, 0);
    contextExchange(0, processManager->processTable->executingArray);
    processManager->processTable->processStateArray[0] = RUNNING;
    processManager->kill = 0;
    processManager->freeID = 0;

    return processManager;
}

//(B n)Bloqueia o processo na CPU em N unidades de tempo
void blockProcess(ProcessManager *comp, int blockT)
{
    int go_exec;
    int go_block;
    if (comp->processTable->executingArray == 0 || comp->processTable->executingArray == NULL)
    {
        return;
    }
    //Operação em tabela-sai de readyArray e vai para executando
    go_block = *comp->processTable->executingArray;
    go_exec = nextReady(comp->processTable->readyArray);
    contextExchange(go_exec, comp->processTable->executingArray);
    removeFromReadyQueue(comp->processTable->readyArray, go_exec);
    //Operação em tabela-sai de executando e vai para block
    insertBlockedId(comp->processTable->blockedArray, go_block, blockT);
    //Operação real
    int i = searchByIdInProcessTable(go_exec, comp->processTable);
    changeProcess(comp->cpu, comp->processTable->processArray[i], comp->processTable->programCounterArray[i],
                  **comp->processTable->CPUTimerArray, 0);
    comp->processTable->processStateArray[go_exec] = RUNNING;
    comp->processTable->processStateArray[go_block] = BLOCKED;
}

void scheduleProcess(ProcessManager *comp)
{
    int go_exec;
    int go_ready;
    int time = 1;
    //Operação em tabela-sai de readyArray e vai para executando
    go_ready = *comp->processTable->executingArray;
    go_exec = nextReady(comp->processTable->readyArray);
    if (go_exec == -1)
    {
        return;
    }
    comp->processTable->priorityIdArray[*comp->processTable->executingArray]++;
    contextExchange(go_exec, comp->processTable->executingArray);
    removeFromReadyQueue(comp->processTable->readyArray, go_exec);
    //Operação em tabela-sai de executando e vai para pronto
    int i = searchByIdInProcessTable(go_exec, comp->processTable);
    int j = searchByIdInProcessTable(go_ready, comp->processTable);
    printf("ID ARRAY: %d\n", comp->processTable->idArray[i]);
    printf("PRIOr ID: %d\n", comp->processTable->priorityIdArray[j]);
    insertToReadyQueue(comp->processTable->readyArray, go_ready,
                       comp->processTable->priorityIdArray[j]);

    printProcessTable(comp->processTable);
    //Operação real
    if (pow(2, (4 - comp->processTable->priorityIdArray[i]) - 1) >= 1)
    {
        time = (int) pow(2, (4 - comp->processTable->priorityIdArray[i]) - 1);
    }
    printf("\n\n\n\n%d\n\n\n\n", time);
    changeProcess(comp->cpu, comp->processTable->processArray[i], comp->processTable->programCounterArray[i], time, 0);
    comp->processTable->processStateArray[go_exec] = RUNNING;
    comp->processTable->processStateArray[go_ready] = READY;
    printProcessTable(comp->processTable);
}

//T termina o processo simulado atual e passa o cpu para o proximo processo pronto
void endProcess(ProcessManager *comp)
{
    int go_excl;
    int go_exec;
    if (comp->processTable->executingArray <= 0 || comp->processTable->executingArray == NULL)
    {
        return;
    }
    //Operação em tabela-sai de readyArray e vai para executando
    go_excl = *comp->processTable->executingArray;
    go_exec = nextReady(comp->processTable->readyArray);
    contextExchange(go_exec, comp->processTable->executingArray);
    removeFromReadyQueue(comp->processTable->readyArray, go_exec);
    //Sai de Ready e vai para executingArray

    //Operação real
    int i = searchByIdInProcessTable(go_exec, comp->processTable);
    if (i != -1)
    {
        changeProcess(comp->cpu, comp->processTable->processArray[i], comp->processTable->programCounterArray[i],
                      **comp->processTable->CPUTimerArray, 0);
        comp->processTable->processStateArray[go_exec] = RUNNING;
    }
    //exclui processo da tabela de processos
    comp->processTable->executingArray = NULL;
    comp->cpu->runningProcess = NULL;
    deleteProcessTableProcess(go_excl, comp->processTable);
}

void killComputer(ProcessManager *comp)
{
    freeCPU(comp->cpu);
    deleteProcessTable(comp->processTable);
    comp->timer = 0;
    comp->kill = 1;
    comp->freeID = 0;
}

void execute(ProcessManager *comp)
{
    int go_exec;
    //Operação sob a tabela Ready
    go_exec = nextReady(comp->processTable->readyArray);
    if (go_exec == -1 || comp->processTable->executingArray == NULL)
    {
        printf("There is nothing more to execute\n");
        return;
    }
    contextExchange(go_exec, comp->processTable->executingArray);
    removeFromReadyQueue(comp->processTable->readyArray, go_exec);
    //Operação Real
    int i = searchByIdInProcessTable(go_exec, comp->processTable);
    changeProcess(comp->cpu, comp->processTable->processArray[i],
                  comp->processTable->programCounterArray[i],
                  **comp->processTable->CPUTimerArray, 0);
    comp->processTable->processStateArray[go_exec] = RUNNING;
}

void processExecuting(ProcessManager *processManager)
{
    //search for a proces while cpu is ampity
    if (processManager->processTable->executingArray < 0 || !processManager->processTable->executingArray ||
        !processManager->cpu->runningProcess)
    {
        //terminate the Computer if kill switch is equal one
        execute(processManager);
        if (processManager->kill == 1)
        {
            killComputer(processManager);
            return;
        }
        //if cpu isn't ampity check the cpu time and escalonate
    }
    else
    {
        //printf("\n\n\n\n%d\n\n\n\n", processManager->cpu->executing_timer);
        //printf("\n\n\n\n%d\n\n\n\n", processManager->cpu->program_timer);
        if (processManager->cpu->quantum >= processManager->cpu->quantumUsed)
        {
            scheduleProcess(processManager);
        }

        //Interpreta o processo aumenta o timer
        uperInterpreter(processManager);
        clockUpPC(processManager);
        processUnblock(processManager);
        printState(processManager->processTable->readyArray);
        //Check the kill switch
        if (processManager->kill == 1)
        {
            killComputer(processManager);
            return;
        }
    }
}

void processUnblock(ProcessManager *comp)
{
    int go_ready;
    for (int i = 0; i < comp->processTable->tableCapacity; ++i)
    {
        if (comp->processTable->blockedArray->blockTimes[i] == 0)
        {
            go_ready = comp->processTable->blockedArray->ids[i];
            removeBlockedId(comp->processTable->blockedArray, go_ready);
            insertToReadyQueue(comp->processTable->readyArray, go_ready,
                               comp->processTable->priorityIdArray[searchByIdInProcessTable(go_ready,
                                                                                            comp->processTable)]);
            comp->processTable->processStateArray[searchByIdInProcessTable(go_ready, comp->processTable)] = READY;
        }
    }
}

void clockUpPC(ProcessManager *comp)
{
    timeUp(comp->timer);
    timeUp(comp->cpu->quantum);
    blockDownClock(comp->processTable->blockedArray);
}


void processCP(ProcessManager *processManager, Process *process, int PcPlus)
{

    process = generateProcessCopy(processManager->cpu->runningProcess);
    copyProcess(processManager->processTable, process, processManager->timer, PcPlus);
}

void processRewind(ProcessManager *comp, char *arq)
{
    freeProcess(comp->processTable->processArray[*comp->processTable->executingArray]);
    comp->processTable->processArray[*comp->processTable->executingArray] = initializeProcessFromFile(arq);
}

void attExec(ProcessManager *comp)
{
    comp->processTable->programCounterArray[*comp->processTable->executingArray] = comp->cpu->programCounter;
    comp->processTable->processArray[*comp->processTable->executingArray] = comp->cpu->runningProcess;
    comp->processTable->CPUTimerArray[*comp->processTable->executingArray]++;
}

void uperInterpreter(ProcessManager *comp)
{
    int blk;
    Process *process;
    int PcPlus;
    char **file;
    printf("PC: %d\n", comp->cpu->programCounter);
    int cpuResp = interpreter(comp->cpu, &blk, file, &PcPlus);
    comp->cpu->programCounter++;
    attExec(comp);
    switch (cpuResp)
    {
        case 0:
            return;
        case 1://Bloqueia esse processo simulado por n unidades de tempo->
            blockProcess(comp, blk);
            return;
        case 2://Acaba com o processo atual e coloca outro no lugar
            endProcess(comp);
            return;
        case 3://Crian um novo processo com base no atual do CPU
            processCP(comp, process, PcPlus);
            return;
        case 4://Recria o processo atual com base em um arquivo
            processRewind(comp, *file);
            return;
        default:
            return;
    }
}