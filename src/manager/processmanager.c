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
void blockProcess(ProcessManager *processManager, int blockTime)
{
    int go_exec;
    int go_block;
    if (processManager->processTable->executingArray == 0 || processManager->processTable->executingArray == NULL)
    {
        return;
    }
    //Operação em tabela-sai de readyArray e vai para executando
    go_block = *processManager->processTable->executingArray;
    go_exec = nextReady(processManager->processTable->readyArray);
    contextExchange(go_exec, processManager->processTable->executingArray);
    removeFromReadyQueue(processManager->processTable->readyArray, go_exec);
    //Operação em tabela-sai de executando e vai para block
    insertBlockedId(processManager->processTable->blockedArray, go_block, blockTime);
    //Operação real
    int i = searchByIdInProcessTable(go_exec, processManager->processTable);
    changeProcess(processManager->cpu, processManager->processTable->processArray[i],
                  processManager->processTable->programCounterArray[i],
                  **processManager->processTable->CPUTimerArray, 0);
    processManager->processTable->processStateArray[go_exec] = RUNNING;
    processManager->processTable->processStateArray[go_block] = BLOCKED;
}

void scheduleProcess(ProcessManager *processManager)
{
    int go_exec;
    int go_ready;
    int time = 1;
    //Operação em tabela-sai de readyArray e vai para executando
    go_ready = *processManager->processTable->executingArray;
    go_exec = nextReady(processManager->processTable->readyArray);
    if (go_exec == -1)
    {
        return;
    }
    processManager->processTable->priorityIdArray[*processManager->processTable->executingArray]++;
    contextExchange(go_exec, processManager->processTable->executingArray);
    removeFromReadyQueue(processManager->processTable->readyArray, go_exec);
    //Operação em tabela-sai de executando e vai para pronto
    int i = searchByIdInProcessTable(go_exec, processManager->processTable);
    int j = searchByIdInProcessTable(go_ready, processManager->processTable);
    printf("ID ARRAY: %d\n", processManager->processTable->idArray[i]);
    printf("PRIOr ID: %d\n", processManager->processTable->priorityIdArray[j]);
    insertToReadyQueue(processManager->processTable->readyArray, go_ready,
                       processManager->processTable->priorityIdArray[j]);

    printProcessTable(processManager->processTable);
    //Operação real
    if (pow(2, (4 - processManager->processTable->priorityIdArray[i]) - 1) >= 1)
    {
        time = (int) pow(2, (4 - processManager->processTable->priorityIdArray[i]) - 1);
    }
    printf("\n\n\n\n%d\n\n\n\n", time);
    changeProcess(processManager->cpu, processManager->processTable->processArray[i],
                  processManager->processTable->programCounterArray[i], time, 0);
    processManager->processTable->processStateArray[go_exec] = RUNNING;
    processManager->processTable->processStateArray[go_ready] = READY;
    printProcessTable(processManager->processTable);
}

//T termina o processo simulado atual e passa o cpu para o proximo processo pronto
void endProcess(ProcessManager *processManager)
{
    int go_excl;
    int go_exec;
    if (processManager->processTable->executingArray <= 0 || processManager->processTable->executingArray == NULL)
    {
        return;
    }
    //Operação em tabela-sai de readyArray e vai para executando
    go_excl = *processManager->processTable->executingArray;
    go_exec = nextReady(processManager->processTable->readyArray);
    contextExchange(go_exec, processManager->processTable->executingArray);
    removeFromReadyQueue(processManager->processTable->readyArray, go_exec);
    //Sai de Ready e vai para executingArray

    //Operação real
    int i = searchByIdInProcessTable(go_exec, processManager->processTable);
    if (i != -1)
    {
        changeProcess(processManager->cpu, processManager->processTable->processArray[i],
                      processManager->processTable->programCounterArray[i],
                      **processManager->processTable->CPUTimerArray, 0);
        processManager->processTable->processStateArray[go_exec] = RUNNING;
    }
    //exclui processo da tabela de processos
    processManager->processTable->executingArray = NULL;
    processManager->cpu->runningProcess = NULL;
    deleteProcessTableProcess(go_excl, processManager->processTable);
}

void killComputer(ProcessManager *processManager)
{
    freeCPU(processManager->cpu);
    deleteProcessTable(processManager->processTable);
    processManager->timer = 0;
    processManager->kill = 1;
    processManager->freeID = 0;
}

void execute(ProcessManager *processManager)
{
    int go_exec;
    //Operação sob a tabela Ready
    go_exec = nextReady(processManager->processTable->readyArray);
    if (go_exec == -1 || processManager->processTable->executingArray == NULL)
    {
        printf("There is nothing more to execute\n");
        return;
    }
    contextExchange(go_exec, processManager->processTable->executingArray);
    removeFromReadyQueue(processManager->processTable->readyArray, go_exec);
    //Operação Real
    int i = searchByIdInProcessTable(go_exec, processManager->processTable);
    changeProcess(processManager->cpu, processManager->processTable->processArray[i],
                  processManager->processTable->programCounterArray[i],
                  **processManager->processTable->CPUTimerArray, 0);
    processManager->processTable->processStateArray[go_exec] = RUNNING;
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

void processUnblock(ProcessManager *processManager)
{
    int go_ready;
    for (int i = 0; i < processManager->processTable->tableCapacity; ++i)
    {
        if (processManager->processTable->blockedArray->blockTimes[i] == 0)
        {
            go_ready = processManager->processTable->blockedArray->ids[i];
            removeBlockedId(processManager->processTable->blockedArray, go_ready);
            insertToReadyQueue(processManager->processTable->readyArray, go_ready,
                               processManager->processTable->priorityIdArray[searchByIdInProcessTable(go_ready,
                                                                                                      processManager->processTable)]);
            processManager->processTable->processStateArray[searchByIdInProcessTable(go_ready,
                                                                                     processManager->processTable)] = READY;
        }
    }
}

void clockUpPC(ProcessManager *processManager)
{
    timeUp(processManager->timer);
    timeUp(processManager->cpu->quantum);
    blockDownClock(processManager->processTable->blockedArray);
}


void processCP(ProcessManager *processManager, Process *process, int PcPlus)
{

    process = generateProcessCopy(processManager->cpu->runningProcess);
    copyProcess(processManager->processTable, process, processManager->timer, PcPlus);
}

void processRewind(ProcessManager *processManager, char *filename)
{
    freeProcess(processManager->processTable->processArray[*processManager->processTable->executingArray]);
    processManager->processTable->processArray[*processManager->processTable->executingArray] = initializeProcessFromFile(
            filename);
}

void attExec(ProcessManager *processManager)
{
    processManager->processTable->programCounterArray[*processManager->processTable->executingArray] = processManager->cpu->programCounter;
    processManager->processTable->processArray[*processManager->processTable->executingArray] = processManager->cpu->runningProcess;
    processManager->processTable->CPUTimerArray[*processManager->processTable->executingArray]++;
}

void uperInterpreter(ProcessManager *processManager)
{
    int blk;
    Process *process;
    int PcPlus;
    char **file;
    printf("PC: %d\n", processManager->cpu->programCounter);
    int cpuResp = interpreter(processManager->cpu, &blk, file, &PcPlus);
    processManager->cpu->programCounter++;
    attExec(processManager);
    switch (cpuResp)
    {
        case 0:
            return;
        case 1://Bloqueia esse processo simulado por n unidades de tempo->
            blockProcess(processManager, blk);
            return;
        case 2://Acaba com o processo atual e coloca outro no lugar
            endProcess(processManager);
            return;
        case 3://Crian um novo processo com base no atual do CPU
            processCP(processManager, process, PcPlus);
            return;
        case 4://Recria o processo atual com base em um arquivo
            processRewind(processManager, *file);
            return;
        default:
            return;
    }
}