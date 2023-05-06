#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../input/input.h"
#include "../error/error.h"
#include "../printer/printer.h"

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

    initializeTimer(&processManager->timer);
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

    if (!addProcessTableProcess(processManager->processTable, filename, -1, 0))
    {
        printFullQueue();
    }

    if (!removeFromReadyQueue(processManager->processTable->readyArray, 0))
    {
        printProcessNotFound();
    }

    contextExchange(0, processManager->processTable->executingArray);
    processManager->processTable->processStateArray[0] = RUNNING;
    processManager->kill = 0;
    processManager->freeID = 0;

    return processManager;
}


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

    if (go_exec == -1)
    {
        printEmptyQueue();
    }

    contextExchange(go_exec, processManager->processTable->executingArray);
    if (!removeFromReadyQueue(processManager->processTable->readyArray, go_exec))
    {
        printProcessNotFound();
    }

    //Operação em tabela-sai de executando e vai para block
    insertBlockedId(processManager->processTable->blockedArray, go_block, blockTime);
    //Operação real
    int i = searchByIdInProcessTable(go_exec, processManager->processTable);
    changeProcess(processManager->cpu, processManager->processTable->processArray[i],
                  processManager->processTable->programCounterArray[i],
                  *processManager->processTable->CPUTimerArray, 0);
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
    if (processManager->processTable->priorityIdArray[*processManager->processTable->executingArray] < 3)
    {
        processManager->processTable->priorityIdArray[*processManager->processTable->executingArray]++;
    }
    contextExchange(go_exec, processManager->processTable->executingArray);
    if (!removeFromReadyQueue(processManager->processTable->readyArray, go_exec))
    {
        printProcessNotFound();
    }
    //Operação em tabela-sai de executando e vai para pronto
    int i = searchByIdInProcessTable(go_exec, processManager->processTable);
    int j = searchByIdInProcessTable(go_ready, processManager->processTable);
    printProcessTable(processManager->processTable);
    if (!insertToReadyQueue(processManager->processTable->readyArray, go_ready,
                            processManager->processTable->priorityIdArray[j]))
    {
        printFullQueue();
    }

    //Operação real
    if (pow(2, (4 - processManager->processTable->priorityIdArray[i]) - 1) >= 1)
    {
        time = (int) pow(2, (4 - processManager->processTable->priorityIdArray[i]) - 1);
    }

    changeProcess(processManager->cpu, processManager->processTable->processArray[i],
                  processManager->processTable->programCounterArray[i],
                  time,
                  0);
    processManager->processTable->processStateArray[go_exec] = RUNNING;
    processManager->processTable->processStateArray[go_ready] = READY;
}


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

    if (go_exec == -1)
    {
        printFullQueue();
    }
    else
    {
        contextExchange(go_exec, processManager->processTable->executingArray);
        if (!removeFromReadyQueue(processManager->processTable->readyArray, go_exec))
        {
            printProcessNotFound();
        }
    }

    //Sai de Ready e vai para executingArray

    //Operação real
    int i = searchByIdInProcessTable(go_exec, processManager->processTable);
    if (i != -1)
    {
        changeProcess(processManager->cpu, processManager->processTable->processArray[i],
                      processManager->processTable->programCounterArray[i],
                      *processManager->processTable->CPUTimerArray, 0);
        processManager->processTable->processStateArray[go_exec] = RUNNING;
    }
    else
    {
        //exclui processo da tabela de processos
        processManager->processTable->executingArray = NULL;
        processManager->cpu->runningProcess = NULL;
    }

    deleteProcessTableProcess(go_excl, processManager->processTable);
}


void execute(ProcessManager *processManager)
{
    int go_exec;
    //Operação sob a tabela Ready
    go_exec = nextReady(processManager->processTable->readyArray);
    if (go_exec == -1 || processManager->processTable->executingArray == NULL)
    {
        printFinishExe();
        return;
    }
    contextExchange(go_exec, processManager->processTable->executingArray);
    if (!removeFromReadyQueue(processManager->processTable->readyArray, go_exec))
    {
        printProcessNotFound();
    }

    //Operação Real
    int i = searchByIdInProcessTable(go_exec, processManager->processTable);
    changeProcess(processManager->cpu, processManager->processTable->processArray[i],
                  processManager->processTable->programCounterArray[i],
                  *processManager->processTable->CPUTimerArray, 0);
    processManager->processTable->processStateArray[go_exec] = RUNNING;
}


void processExecuting(ProcessManager *processManager)
{
    //search for a proces while cpu is ampity
    if (processManager->processTable->executingArray < 0 || processManager->processTable->executingArray == NULL ||
        processManager->cpu->runningProcess == NULL)
    {
        //terminate the Computer if kill switch is equal one
        execute(processManager);
        if (processManager->kill == 1)
        {
            freeProcessManager(processManager);
            return;
        }
        //if cpu isn't ampity check the cpu time and escalonate
    }
    else
    {
        if (processManager->cpu->executing_timer >= processManager->cpu->program_timer)
        {
            scheduleProcess(processManager);
        }

        //Interpreta o processo aumenta o timer
        upperInterpreter(processManager);
        clockUpPC(processManager);
        processUnblock(processManager);
        printState(processManager->processTable->readyArray);
        //Check the kill switch
        if (processManager->kill == 1)
        {
            freeProcessManager(processManager);
            return;
        }
    }
}


void processUnblock(ProcessManager *processManager)
{
    int go_ready;
    for (int i = 0; i < processManager->processTable->tableCapacity; i++)
    {
        if (processManager->processTable->blockedArray->blockTimes[i] == 0)
        {
            go_ready = processManager->processTable->blockedArray->ids[i];
            removeBlockedId(processManager->processTable->blockedArray, go_ready);
            if (!insertToReadyQueue(processManager->processTable->readyArray, go_ready,
                                    processManager->processTable->priorityIdArray[searchByIdInProcessTable(go_ready,
                                                                                                           processManager->processTable)]))
            {
                printFullQueue();
            }
            processManager->processTable->processStateArray[searchByIdInProcessTable(go_ready,
                                                                                     processManager->processTable)] = READY;
        }
    }
}


void clockUpPC(ProcessManager *processManager)
{
    timeUp(&processManager->timer);
    timeUp(&processManager->cpu->executing_timer);
    blockDownClock(processManager->processTable->blockedArray);
}


void processCP(ProcessManager *processManager, Process *process, int PcPlus)
{
    process = generateProcessCopy(processManager->cpu->runningProcess);
    if (!copyProcess(processManager->processTable, process, processManager->timer, PcPlus))
    {
        printFullQueue();
    }
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


void upperInterpreter(ProcessManager *processManager)
{
    int blk;
    Process *proc;
    int PcPlus;
    char **arq;
    int cpuResp = interpreter(processManager->cpu, &blk, arq, &PcPlus);
    processManager->cpu->programCounter++;
    attExec(processManager);
    switch (cpuResp)
    {
        case 0:
            return;
        case 1://Bloqueia esse processo simulado por n unidades de tempo.
            blockProcess(processManager, blk);
            return;
        case 2://Acaba com o processo atual e coloca outro no lugar
            endProcess(processManager);
            return;
        case 3://Crian um novo processo com base no atual do CPU
            processCP(processManager, proc, PcPlus);
            return;
        case 4://Recria o processo atual com base em um arquivo
            processRewind(processManager, *arq);
            return;
        default:
            return;
    }
}


void freeProcessManager(ProcessManager *processManager)
{
    freeCPU(processManager->cpu);
    deleteProcessTable(processManager->processTable);
    processManager->timer = 0;
    processManager->kill = 1;
    processManager->freeID = 0;
}