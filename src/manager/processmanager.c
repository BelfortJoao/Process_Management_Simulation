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

    contextExchange(0, &processManager->processTable->running);
    processManager->processTable->processTableCellQueue->front->processTableCell->state = RUNNING;
    processManager->kill = false;

    return processManager;
}


void blockProcess(ProcessManager *processManager, int blockTime)
{
    if (processManager->processTable->running == 0)
    {
        return;
    }

    //Operação em tabela-sai de readyArray e vai para executando
    int processToRunId = nextProcessReady(processManager->processTable->readyArray);

    if (processToRunId == -1)
    {
        printEmptyQueue();
    }

    contextExchange(processToRunId, &processManager->processTable->running);

    if (!removeFromReadyQueue(processManager->processTable->readyArray, processToRunId))
    {
        printProcessNotFound();
    }

    //Operação em tabela-sai de executando e vai para block
    insertBlockedId(processManager->processTable->blockedArray,
                    processManager->processTable->running,
                    blockTime);

    //Operação real
    ProcessTableCell *processToRun = getProcessTableCellByProcessId(
            processManager->processTable->processTableCellQueue,
            processToRunId);

    getProcessTableCellByProcessId(processManager->processTable->processTableCellQueue,
                                   processManager->processTable->running)->state = BLOCKED;

    changeProcess(processManager->cpu,
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
            processManager->processTable->running);
    ProcessTableCell *processToRunCell = getProcessTableCellByProcessId(
            processManager->processTable->processTableCellQueue,
            nextProcessReady(
                    processManager->processTable->readyArray));

    if (!processToRunCell)
    {
        return;
    }

    // WTF
    if (processToReadyCell->priority < 3)
    {
        processToReadyCell->priority++;
    }

    contextExchange(processToRunCell->id, &processManager->processTable->running);

    if (!removeFromReadyQueue(processManager->processTable->readyArray, processToRunCell->id))
    {
        printProcessNotFound();
    }

    /// Remove when in production mode.
    printProcessTable(processManager->processTable);

    if (!insertToReadyQueue(processManager->processTable->readyArray,
                            processToReadyCell->id,
                            processToReadyCell->priority))
    {
        printFullQueue();
    }

    int time = 1;

    if (pow(2, (4 - processToRunCell->priority) - 1) >= 1)
    {
        time = (int) pow(2, (4 - processToRunCell->priority) - 1);
    }

    changeProcess(processManager->cpu,
                  processToRunCell->process,
                  processToRunCell->programCounter,
                  time,
                  0);

    processToRunCell->state = RUNNING;
    processToReadyCell->state = READY;
}


void endProcess(ProcessManager *processManager)
{
    if (processManager->processTable->running == -1)
    {
        return;
    }

    int go_excl = processManager->processTable->running;
    int processToRunId = nextProcessReady(processManager->processTable->readyArray);

    if (processToRunId == -1)
    {
        printFullQueue();
    }
    else
    {
        contextExchange(processToRunId, &processManager->processTable->running);

        if (!removeFromReadyQueue(processManager->processTable->readyArray, processToRunId))
        {
            printProcessNotFound();
        }
    }

    //Sai de Ready e vai para running

    ProcessTableCell *processToRunCell = getProcessTableCellByProcessId(
            processManager->processTable->processTableCellQueue,
            processToRunId);

    if (processToRunCell)
    {
        changeProcess(processManager->cpu,
                      processToRunCell->process,
                      processToRunCell->programCounter,
                      processToRunCell->CPUTime,
                      0);
        processToRunCell->state = RUNNING;
    }
    else
    {
        processManager->processTable->running = -1;
        processManager->cpu->runningProcess = NULL;
    }

    deleteProcessTableProcess(go_excl, processManager->processTable);
}


void execute(ProcessManager *processManager)
{
    int processToRunId = nextProcessReady(processManager->processTable->readyArray);

    if (processToRunId == -1 || processManager->processTable->running == -1)
    {
        printFinishExe();
        return;
    }

    contextExchange(processToRunId, &processManager->processTable->running);


    if (!removeFromReadyQueue(processManager->processTable->readyArray, processToRunId))
    {
        printProcessNotFound();
    }

    //Operação Real
    ProcessTableCell *processToRunCell = getProcessTableCellByProcessId(
            processManager->processTable->processTableCellQueue,
            processToRunId);

    changeProcess(processManager->cpu,
                  processToRunCell->process,
                  processToRunCell->programCounter,
                  processToRunCell->CPUTime,
                  0);

    processToRunCell->state = RUNNING;
}


void processExecuting(ProcessManager *processManager)
{
    if (processManager->processTable->running < 0 || processManager->cpu->runningProcess == NULL)
    {
        execute(processManager);

        if (processManager->kill)
        {
            freeProcessManager(processManager);
        }

        return;
    }

    if (processManager->cpu->executing_timer >= processManager->cpu->program_timer)
    {
        scheduleProcess(processManager);
    }

    upperInterpreter(processManager);
    clockUpPC(processManager);
    processUnblock(processManager);
    printState(processManager->processTable->readyArray);

    if (processManager->kill)
    {
        freeProcessManager(processManager);
    }
}


void processUnblock(ProcessManager *processManager)
{
    /// TODO: Blocked has to be a queue.
    for (int i = 0; i < 10; i++)
    {
        if (processManager->processTable->blockedArray->blockTimes[i] == 0)
        {
            ProcessTableCell *processToReadyCell = getProcessTableCellByProcessId(
                    processManager->processTable->processTableCellQueue,
                    processManager->processTable->blockedArray->ids[i]);

            removeBlockedId(processManager->processTable->blockedArray, processToReadyCell->id);

            if (!insertToReadyQueue(processManager->processTable->readyArray,
                                    processToReadyCell->id,
                                    processToReadyCell->priority))
            {
                printFullQueue();
            }

            processToReadyCell->state = READY;
        }
    }
}


void clockUpPC(ProcessManager *processManager)
{
    timeUp(&processManager->timer);
    timeUp(&processManager->cpu->executing_timer);
    blockDownClock(processManager->processTable->blockedArray);
}


void processCP(ProcessManager *processManager, int PcPlus)
{
//    if (!copyProcess(processManager->processTable, processManager->timer, PcPlus))
//    {
//        printFullQueue();
//    }
}


void processRewind(ProcessManager *processManager, char *filename)
{
    removeFromProcessTableQueue(processManager->processTable->processTableCellQueue,
                                processManager->processTable->running);

    insertToProcessTableQueue(processManager->processTable->processTableCellQueue, filename, -1, 0);
}


void attExec(ProcessManager *processManager)
{
    ProcessTableCell *runningProcessCell = getProcessTableCellByProcessId(
            processManager->processTable->processTableCellQueue,
            processManager->processTable->running);

    runningProcessCell->programCounter = processManager->cpu->programCounter;
    runningProcessCell->CPUTime++;

    // This doesn't make sense.
    //processManager->processTable->processArray[*processManager->processTable->running] = processManager->cpu->runningProcess;
}


void upperInterpreter(ProcessManager *processManager)
{
    int blockTime;
    int PcPlus;
    char **filename;
    int cpuResp = interpreter(processManager->cpu, &blockTime, filename, &PcPlus);

    processManager->cpu->programCounter++;
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
        freeCPU(processManager->cpu);
        freeProcessTable(processManager->processTable);
        free(processManager);
    }
}