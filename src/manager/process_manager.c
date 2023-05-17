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


ProcessManager *initializeProcessManagerFromFile(char *filename, int numberCores)
{
    ProcessManager *processManager = initializeProcessManager();

    if (!processManager)
    {
        return NULL;
    }

    initializeTimer(&processManager->timer);
    processManager->cpuQueue = initializeCPU(filename, numberCores);

    if (!processManager->cpuQueue)
    {
        return NULL;
    }

    processManager->processTable = initializeProcessTable(DEFAULT_INITIAL_CAPACITY);

    if (!addProcessTableProcess(processManager->processTable, filename, -1, 0))
    {
        printFullQueue();
    }

    if (!removeFromReady(processManager->processTable->ready, 0))
    {
        printProcessNotFound();
    }

    contextExchange(0, &processManager->processTable->runningId);
    processManager->processTable->processTableCellQueue->front->processTableCell->state = RUNNING;
    processManager->kill = false;

    return processManager;
}


void blockProcess(ProcessManager *processManager, int blockTime)
{
    if (processManager->processTable->runningId < 0)
    {
        return;
    }

    //Operação em tabela-sai de ready e vai para executando
    int processToRunId = nextProcessReady(processManager->processTable->ready);
    int processToBlock = processManager->processTable->runningId;

    if (processToRunId == -1)
    {
        printEmptyQueue();
        return;
    }

    contextExchange(processToRunId, &processManager->processTable->runningId);

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

    changeProcess(processManager->cpuQueue->front,
                  processToRun->process,
                  processToRun->programCounter,
                  processToRun->CPUTime,
                  0);

    processToRun->state = RUNNING;
}


void scheduleProcess(ProcessManager *processManager)
{
    // Ready array -> Running.
    ProcessTableCell *processToReadyCell = getProcessTableCellByProcessId(
            processManager->processTable->processTableCellQueue,
            processManager->processTable->runningId);
    ProcessTableCell *processToRunCell = getProcessTableCellByProcessId(
            processManager->processTable->processTableCellQueue,
            nextProcessReady(
                    processManager->processTable->ready));

    if (!processToRunCell)
    {
        return;
    }

    // WTF
    if (processToReadyCell->priority < 3)
    {
        processToReadyCell->priority++;
    }

    contextExchange(processToRunCell->id, &processManager->processTable->runningId);

    if (!removeFromReady(processManager->processTable->ready, processToRunCell->id))
    {
        printProcessNotFound();
    }

    /// Remove when in production mode.
    printProcessTable(processManager->processTable);

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

    changeProcess(processManager->cpuQueue->front,
                  processToRunCell->process,
                  processToRunCell->programCounter,
                  time,
                  0);

    processToRunCell->state = RUNNING;
    processToReadyCell->state = READY;
}


void endProcess(ProcessManager *processManager)
{
    if (processManager->processTable->runningId == -1)
    {
        return;
    }

    int processIdToDelete = processManager->processTable->runningId;
    int processToRunId = nextProcessReady(processManager->processTable->ready);

    if (processToRunId == -1)
    {
        printFullQueue();
    }
    else
    {
        contextExchange(processToRunId, &processManager->processTable->runningId);

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

        changeProcess(processManager->cpuQueue->front,
                      processToRunCell->process,
                      processToRunCell->programCounter,
                      time,
                      0);

        processToRunCell->state = RUNNING;
    }
    else
    {
        processManager->processTable->runningId = -1;
        processManager->cpuQueue->front->runningProcess = NULL;
    }

    deleteProcessTableProcess(processIdToDelete, processManager->processTable);
}


void execute(ProcessManager *processManager)
{
    int processToRunId = nextProcessReady(processManager->processTable->ready);

    if (processToRunId == -1)
    {
        printFinishExe();
        return;
    }

    contextExchange(processToRunId, &processManager->processTable->runningId);


    if (!removeFromReady(processManager->processTable->ready, processToRunId))
    {
        printProcessNotFound();
    }

    //Operação Real
    ProcessTableCell *processToRunCell = getProcessTableCellByProcessId(
            processManager->processTable->processTableCellQueue,
            processToRunId);

    changeProcess(processManager->cpuQueue->front,
                  processToRunCell->process,
                  processToRunCell->programCounter,
                  processToRunCell->CPUTime,
                  0);

    processToRunCell->state = RUNNING;
}


void processExecuting(ProcessManager *processManager)
{
    if (processManager->processTable->runningId < 0 || processManager->cpuQueue->front->runningProcess == NULL)
    {
        execute(processManager);
        processUnblock(processManager);
        if (processManager->kill)
        {
            freeProcessManager(processManager);
        }

        return;
    }

    if (processManager->cpuQueue->front->executing_timer >= processManager->cpuQueue->front->program_timer)
    {
        scheduleProcess(processManager);
    }
    processUnblock(processManager);
    upperInterpreter(processManager);
    clockUpPC(processManager);
    printState(processManager->processTable->ready);

    if (processManager->kill)
    {
        freeProcessManager(processManager);
    }
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
    timeUp(&processManager->cpuQueue->front->executing_timer);
}


void processCP(ProcessManager *processManager, int PcPlus)
{
    if (!copyProcess(processManager->processTable, processManager->timer, PcPlus))
    {
        printFullQueue();
    }
    {
        printFullQueue();
    }
}


void processRewind(ProcessManager *processManager, char *filename)
{
    removeFromProcessTableQueue(processManager->processTable->processTableCellQueue,
                                processManager->processTable->runningId);

    insertToProcessTableQueue(processManager->processTable->processTableCellQueue, filename, -1, 0);
}


void attExec(ProcessManager *processManager)
{
    ProcessTableCell *runningProcessCell = getProcessTableCellByProcessId(
            processManager->processTable->processTableCellQueue,
            processManager->processTable->runningId);

    runningProcessCell->programCounter = processManager->cpuQueue->front->programCounter;
    runningProcessCell->CPUTime++;
}


void upperInterpreter(ProcessManager *processManager)
{
    int blockTime;
    int PcPlus;
    char **filename;
    int cpuResp = interpreter(processManager->cpuQueue->front, &blockTime, filename, &PcPlus);

    processManager->cpuQueue->front->programCounter++;
    attExec(processManager);

    switch (cpuResp)
    {
        case 0:
            break;
        case 1://Bloqueia esse processo simulado por n unidades de tempo.
            blockProcess(processManager, blockTime);
            break;
        case 2://Acaba com o processo atual e coloca outro no lugar
            endProcess(processManager);
            break;
        case 3://Crian um novo processo com base no atual do CPU
            processCP(processManager, PcPlus);
            break;
        case 4://Recria o processo atual com base em um arquivo
            processRewind(processManager, *filename);
            break;
        default:
            break;
    }
}


void freeProcessManager(ProcessManager *processManager)
{
    if (processManager)
    {
        freeCPU(processManager->cpuQueue);
        freeProcessTable(processManager->processTable);
        free(processManager);
    }
}
