#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../error/error.h"
#include "../printer/printer.h"

#include "process_manager.h"


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


ProcessManager *initializeProcessManagerFromFile(char *filename, int numberOfCores)
{
    ProcessManager *processManager = initializeProcessManager();

    if (!processManager)
    {
        return NULL;
    }

    initializeTimer(&processManager->timer);
    processManager->artCounter = initializeArtCounter();
    processManager->cpu = initializeCPU(numberOfCores);

    Core *core = nextIdleCore(processManager->cpu);

    initializeCoreFromFile(core, filename);

    if (!processManager->cpu)
    {
        return NULL;
    }

    processManager->processTable = initializeProcessTable(DEFAULT_INITIAL_CAPACITY, numberOfCores);

    if (!addProcessTableProcess(processManager->processTable, core->runningProcess, -1, processManager->timer))
    {
        printFullQueue();
    }

    if (!removeFromReady(processManager->processTable->ready, 0))
    {
        printProcessNotFound();
    }

    contextExchange(0, processManager->processTable->runningId, 0);
    processManager->processTable->processTableCellQueue->front->processTableCell->state = RUNNING;
    processManager->kill = false;

    return processManager;
}


void blockProcess(ProcessManager *processManager, int blockTime, int typeOfScheduler, int coreNum)
{
    if (processManager->processTable->runningId[coreNum] < 0)
    {
        return;
    }

    //Operação em tabela-sai de ready e vai para executando
    int processToRunId = nextProcessReady(processManager->processTable->ready, typeOfScheduler);
    int processToBlock = processManager->processTable->runningId[coreNum];

    if (processToRunId == -1)
    {
        printEmptyQueue();
        return;
    }

    contextExchange(processToRunId, processManager->processTable->runningId, coreNum);

    if (!removeFromReady(processManager->processTable->ready, processToRunId))
    {
        printProcessNotFound();
    }

    //Operação em tabela-sai de executando e vai para block
    insertToBlockedQueue(processManager->processTable->blockedQueue,
                         processToBlock,
                         blockTime);

    //Operação real
    ProcessTableCell *processToRun = getProcessTableCellByProcessId(
            processManager->processTable->processTableCellQueue,
            processToRunId);

    getProcessTableCellByProcessId(processManager->processTable->processTableCellQueue,
                                   processToBlock)->state = BLOCKED;

    changeProcess(nextIdleCore(processManager->cpu),
                  processToRun->process,
                  processToRun->programCounter,
                  processToRun->CPUTime,
                  0);

    processToRun->state = RUNNING;
}


void scheduleProcess(ProcessManager *processManager, int typeOfScheduler, int coreNum)
{
    if (processManager->processTable->runningId[coreNum] < 0)
    {
        return;
    }
    // Ready array -> Running.
    ProcessTableCell *processToReadyCell = getProcessTableCellByProcessId(
            processManager->processTable->processTableCellQueue,
            processManager->processTable->runningId[coreNum]);
    ProcessTableCell *processToRunCell = getProcessTableCellByProcessId(
            processManager->processTable->processTableCellQueue,
            nextProcessReady(
                    processManager->processTable->ready, typeOfScheduler));

    if (!processToRunCell)
    {
        return;
    }

    processManager->cpu->coreArray[coreNum]->executing_timer = 0;

    if (processToReadyCell->priority < 3)
    {
        processToReadyCell->priority++;
    }

    contextExchange(processToRunCell->id, processManager->processTable->runningId, coreNum);

    if (!removeFromReady(processManager->processTable->ready, processToRunCell->id))
    {
        printProcessNotFound();
    }

    if (!insertToReady(processManager->processTable->ready,
                       processToReadyCell->id,
                       processToReadyCell->priority))
    {
        printFullQueue();
    }

    int time;

    switch (processToRunCell->priority)
    {
        case 0:
            time = 1;
            break;
        case 1:
            time = 2;
            break;
        case 2:
            time = 4;
            break;
        default:
            time = 8;
            break;
    }

    changeProcess(nextIdleCore(processManager->cpu),
                  processToRunCell->process,
                  processToRunCell->programCounter,
                  time,
                  0);

    processToRunCell->state = RUNNING;
    processToReadyCell->state = READY;
}


void endProcess(ProcessManager *processManager, int typeOfScheduler, int coreNum)
{
    if (processManager->processTable->runningId[coreNum] < 0)
    {
        return;
    }

    int processIdToDelete = processManager->processTable->runningId[coreNum];
    int processToRunId = nextProcessReady(processManager->processTable->ready, typeOfScheduler);

    ProcessTableCell *processToFinishCell = getProcessTableCellByProcessId(
            processManager->processTable->processTableCellQueue,
            processIdToDelete);
    addFinishedProcess(processManager->artCounter, processToFinishCell->initialTime, processManager->timer);

    if (processToRunId == -1)
    {
        printFullQueue();
    }
    else
    {
        contextExchange(processToRunId, processManager->processTable->runningId, coreNum);

        if (!removeFromReady(processManager->processTable->ready, processToRunId))
        {
            printProcessNotFound();
        }
    }

    //Sai de Ready e vai para runningId

    ProcessTableCell *processToRunCell = getProcessTableCellByProcessId(
            processManager->processTable->processTableCellQueue,
            processToRunId);

    if (processToRunCell)
    {
        int time;

        switch (processToRunCell->priority)
        {
            case 0:
                time = 1;
                break;
            case 1:
                time = 2;
                break;
            case 2:
                time = 4;
                break;
            default:
                time = 8;
                break;
        }

        if (!changeProcess(processManager->cpu->coreArray[coreNum],
                           processToRunCell->process,
                           processToRunCell->programCounter,
                           time,
                           0))
        {
            processToRunCell->state = READY;
        }
        else
        {
            processToRunCell->state = RUNNING;
        }
    }
    else
    {
        processManager->processTable->runningId[coreNum] = -1;
        processManager->cpu->coreArray[coreNum]->runningProcess->program = NULL;
    }

    deleteProcessTableProcess(processIdToDelete, processManager->processTable);
}


void execute(ProcessManager *processManager, int typeOfScheduler, int coreNum)
{
    int processToRunId = nextProcessReady(processManager->processTable->ready, typeOfScheduler);

    if (processToRunId == -1)
    {
        return;
    }


    if (!removeFromReady(processManager->processTable->ready, processToRunId))
    {
        printProcessNotFound();
    }

    //Operação Real
    ProcessTableCell *processToRunCell = getProcessTableCellByProcessId(
            processManager->processTable->processTableCellQueue,
            processToRunId);

    if (changeProcess(nextIdleCore(processManager->cpu),
                  processToRunCell->process,
                  processToRunCell->programCounter,
                  processToRunCell->CPUTime,
                  0))
    {
        contextExchange(processToRunId, processManager->processTable->runningId, coreNum);
        processToRunCell->state = RUNNING;
    }
}


void processExecuting(ProcessManager *processManager, int typeOfScheduler)
{
    processUnblock(processManager);
    for (int i = 0; i < processManager->cpu->numberOfCores; i++)
    {
        if (processManager->processTable->runningId[i] < 0 ||
            processManager->cpu->coreArray[i]->runningProcess->program == NULL)
        {
            execute(processManager, typeOfScheduler, i);
            if (processManager->kill)
            {
                freeProcessManager(processManager);
            }

            continue;
        }

        Core *core = processManager->cpu->coreArray[i];

        if (core->executing_timer >= core->program_timer)
        {
            scheduleProcess(processManager, typeOfScheduler, i);
        }
        upperInterpreter(processManager, typeOfScheduler, i);

        if (processManager->kill)
        {
            freeProcessManager(processManager);
        }
    }

    clockUpPC(processManager);
}


void processUnblock(ProcessManager *processManager)
{
    //primeiro abaixa o clock
    blockDownClock(processManager->processTable->blockedQueue);

    //pergunta se é igual a zero
    if (processManager->processTable->blockedQueue->front)
    {
        BlockedNode *crrblock = processManager->processTable->blockedQueue->front;

        while (crrblock)
        {
            BlockedNode *nextblock = crrblock->next;

            if (crrblock->blockTime <= 0)
            {
                //coloca em ready
                ProcessTableCell *cell = getProcessTableCellByProcessId(
                        processManager->processTable->processTableCellQueue, crrblock->id);

                insertToReady(processManager->processTable->ready, crrblock->id, cell->priority);
                cell->state = READY;
                //tira de block
                removeFromBlockedQueue(processManager->processTable->blockedQueue, crrblock->id);
            }

            crrblock = nextblock;
        }
    }
}


void clockUpPC(ProcessManager *processManager)
{
    timeUp(&processManager->timer);
    timeUpCPU(processManager->cpu);
}


void processCP(ProcessManager *processManager, int PcPlus, int cpuNum)
{
    if (!copyProcess(processManager->processTable, processManager->timer, PcPlus, cpuNum))
    {
        printFullQueue();
    }
    // coloca o processo na cpu se estiver livre
}


void processRewind(ProcessManager *processManager, char *filename, int coreNum)
{
    removeFromProcessTableQueue(processManager->processTable->processTableCellQueue,
                                processManager->processTable->runningId[coreNum]);

    insertToProcessTableQueue(processManager->processTable->processTableCellQueue, filename, -1, processManager->timer);
}


void attExec(ProcessManager *processManager, int coreNum)
{
    ProcessTableCell *runningProcessCell = getProcessTableCellByProcessId(
            processManager->processTable->processTableCellQueue,
            processManager->processTable->runningId[coreNum]);

    Core *core = processManager->cpu->coreArray[coreNum];

    runningProcessCell->programCounter = core->programCounter;
    runningProcessCell->CPUTime++;
}

void upCPU(CPU *cpu, int coreNum)
{
    if (cpu->coreArray[coreNum]->coreState == WORKING)
    {
        cpu->coreArray[coreNum]->programCounter++;
    }
}

void upperInterpreter(ProcessManager *processManager, int typeOfScheduler, int coreNum)
{
    int blockTime;
    int PcPlus;
    char **filename;

    attExec(processManager, coreNum);

    if (processManager->cpu->coreArray[coreNum]->coreState == WORKING)
    {
        switch (interpreter(processManager->cpu->coreArray[coreNum], &blockTime, filename, &PcPlus))
        {
            case 0:
                break;
            case 1://Bloqueia esse processo simulado por n unidades de tempo.
                blockProcess(processManager, blockTime, typeOfScheduler, coreNum);
                break;
            case 2://Acaba com o processo atual e coloca outro no lugar
                endProcess(processManager, typeOfScheduler, coreNum);
                break;
            case 3://Crian um novo processo com base no atual do Core
                processManager->cpu->coreArray[coreNum]->programCounter += PcPlus;
                processCP(processManager, PcPlus, coreNum);
                break;
            case 4://Recria o processo atual com base em um arquivo
                processRewind(processManager, *filename, coreNum);
                break;
            default:
                break;
        }
    }
    upCPU(processManager->cpu, coreNum);
}


void freeProcessManager(ProcessManager *processManager)
{
    if (processManager)
    {
        freeCPU(processManager->cpu);
        freeProcessTable(processManager->processTable);
        free(processManager);
    }
}