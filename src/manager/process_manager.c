#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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


ProcessManager *initializeProcessManagerFromFile(char *filename)
{
    ProcessManager *processManager = initializeProcessManager();

    if (!processManager)
    {
        return NULL;
    }

    initializeTimer(&processManager->timer);
    processManager->artCounter = initializeArtCounter();
    processManager->cpu = initializeCPU(filename);

    if (!processManager->cpu)
    {
        return NULL;
    }

    processManager->processTable = initializeProcessTable(DEFAULT_INITIAL_CAPACITY);

    if (!addProcessTableProcess(processManager->processTable, filename, -1, processManager->timer))
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
    //printProcessTable(processManager->processTable);

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
    
    int processIdToDelete = processManager->processTable->runningId;
    int processToRunId = nextProcessReady(processManager->processTable->ready);

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
    //printState(processManager->processTable->ready);

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
    timeUp(&processManager->cpu->executing_timer);
}


void processCP(ProcessManager *processManager, int PcPlus)
{
    if (!copyProcess(processManager->processTable, processManager->timer, PcPlus))
    {
        printFullQueue();
    }
}

char** ReadArchive(char* filename) {
    char** lines = NULL;
    char buffer[256];
    int lineCount = 0;

    char cleanedFilename[256];
    int cleanedIndex = 0;

    for (int i = 0; filename[i] != '\0'; i++) {
        if (isalnum(filename[i]) || filename[i] == '.' || filename[i] == '/') {
            cleanedFilename[cleanedIndex++] = filename[i];
        }
    }
    cleanedFilename[cleanedIndex] = '\0';

    char filePath[256];
    sprintf(filePath, "../files/%s", cleanedFilename);
    printf("%s\n",filePath);
    FILE* file = fopen(filePath, "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return NULL;
    }

    // Count the number of lines in the file
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        lineCount++;
    }

    // Allocate memory for the lines
    lines = (char**)malloc(lineCount * sizeof(char*));
    if (lines == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return NULL;
    }

    // Reset the file position
    fseek(file, 0, SEEK_SET);

    // Read each line and store it in the array
    for (int i = 0; i < lineCount; i++) {
        fgets(buffer, sizeof(buffer), file);
        int length = strlen(buffer);
        buffer[length - 1] = '\0'; // Remove the newline character
        lines[i] = (char*)malloc(length * sizeof(char));
        strcpy(lines[i], buffer);
    }

    fclose(file);

    return lines;
}



int numLines(char** newArchive) {
    int count = 0;

    if (newArchive == NULL) {
        printf("Invalid archive.\n");
        return count;
    }

    while (newArchive[count] != NULL) {
        count++;
    }
    printf("%d\n", count);
    return count;
}


void processRewind(ProcessManager* processManager, char* filename) {
    if (processManager == NULL || processManager->cpu == NULL ||
        processManager->cpu->runningProcess == NULL) {
        printf("Invalid process manager or running process.\n");
        return;
    }

    char** newArchive;
    int numlines;

    newArchive = ReadArchive(filename);
    if (newArchive == NULL) {
        printf("Failed to read the archive.\n");
        return;
    }

    numlines = numLines(newArchive);

    reallocateProgram(processManager->cpu->runningProcess, numlines);

    processManager->cpu->runningProcess->program = newArchive;
    processManager->cpu->runningProcess->numLines = numlines;
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