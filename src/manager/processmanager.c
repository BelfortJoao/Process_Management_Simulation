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

    processManager->processTable = initializeProcessTable(DEFAULT_INITIAL_CAPACITY);

    if (!addProcessTableProcess(processManager->processTable, filename, -1, 0))
    {
        printFullQueue();
    }

    if (!removeFromReadyQueue(processManager->processTable->readyArray, 0))
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

    //Operação em tabela-sai de readyArray e vai para executando
    int processToRunId = nextProcessReady(processManager->processTable->readyArray);
    int processToBlock = processManager->processTable->runningId;

    if (processToRunId == -1)
    {
        printEmptyQueue();
    }

    contextExchange(processToRunId, &processManager->processTable->runningId);

    if (!removeFromReadyQueue(processManager->processTable->readyArray, processToRunId))
    {
        printProcessNotFound();
    }

    //Operação em tabela-sai de executando e vai para block
    insertBlockedId(processManager->processTable->blockedArray,
                    processToBlock,
                    blockTime);

    //Operação real
    ProcessTableCell *processToRun = getProcessTableCellByProcessId(
            processManager->processTable->processTableCellQueue,
            processToRunId);

    getProcessTableCellByProcessId(processManager->processTable->processTableCellQueue,
                                   processToBlock)->state = BLOCKED;

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
            processManager->processTable->runningId);
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

    contextExchange(processToRunCell->id, &processManager->processTable->runningId);

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
    int time=0;
    switch (processToRunCell->priority) {
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
    if (processManager->processTable->runningId == -1)
    {
        return;
    }

    int go_excl = processManager->processTable->runningId;
    int processToRunId = nextProcessReady(processManager->processTable->readyArray);

    if (processToRunId == -1)
    {
        printFullQueue();
    }
    else
    {
        contextExchange(processToRunId, &processManager->processTable->runningId);

        if (!removeFromReadyQueue(processManager->processTable->readyArray, processToRunId))
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
        int time=0;
        switch (processToRunCell->priority) {
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
        changeProcess(processManager->cpu,
                      processToRunCell->process,
                      processToRunCell->programCounter,
                        time,
                      0);
        processToRunCell->state = RUNNING;
    }
    else
    {
        processManager->processTable->runningId = -1;
        processManager->cpu->runningProcess = NULL;
    }

    deleteProcessTableProcess(go_excl, processManager->processTable);
}


void execute(ProcessManager *processManager)
{
    int processToRunId = nextProcessReady(processManager->processTable->readyArray);

    if (processToRunId == -1 )
    {
        printFinishExe();
        return;
    }

    contextExchange(processToRunId, &processManager->processTable->runningId);


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
    if (processManager->processTable->runningId < 0 || processManager->cpu->runningProcess == NULL)
    {
        execute(processManager);
        processUnblock(processManager);
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
    processUnblock(processManager);
    upperInterpreter(processManager);
    clockUpPC(processManager);
    printState(processManager->processTable->readyArray);

    if (processManager->kill)
    {
        freeProcessManager(processManager);
    }
}


void processUnblock(ProcessManager *processManager)
{
    //primeiro abaixa o clock
    blockDownClock(processManager->processTable->blockedArray);
    //pergunta se é igual a zero
    if(processManager->processTable->blockedArray->front) {
        BlockNode *crrblock = processManager->processTable->blockedArray->front;
        while (crrblock) {
            BlockNode *nextblock =  crrblock->next;
            if(crrblock->blocked_time<=0){
                //coloca em ready
                ProcessTableCell *cell=  getProcessTableCellByProcessId(
                        processManager->processTable->processTableCellQueue, crrblock->id);

                insertToReadyQueue(processManager->processTable->readyArray, crrblock->id, cell->priority);
                cell->state= READY;
                //tira de block
                removeBlockedId(processManager->processTable->blockedArray,crrblock->id);
            }
            crrblock = nextblock;
        }
    }
}


void clockUpPC(ProcessManager *processManager)
{
    timeUp(&processManager->timer);
    timeUp(&processManager->cpu->executing_timer);
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

    runningProcessCell->programCounter = processManager->cpu->programCounter;
    runningProcessCell->CPUTime++;
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