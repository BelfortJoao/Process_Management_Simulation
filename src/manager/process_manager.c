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


void blockProcess(ProcessManager *processManager, int blockTime, int typeOfScheduler)
{
    if (processManager->processTable->runningId < 0)
    {
        return;
    }

    //Operação em tabela-sai de ready e vai para executando
    int processToRunId = nextProcessReady(processManager->processTable->ready, typeOfScheduler);
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


void scheduleProcess(ProcessManager *processManager, int typeOfScheduler)
{
    // Ready array -> Running.
    ProcessTableCell *processToReadyCell = getProcessTableCellByProcessId(
            processManager->processTable->processTableCellQueue,
            processManager->processTable->runningId);
    ProcessTableCell *processToRunCell = getProcessTableCellByProcessId(
            processManager->processTable->processTableCellQueue,
            nextProcessReady(
                    processManager->processTable->ready, typeOfScheduler));

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


void endProcess(ProcessManager *processManager, int typeOfScheduler)
{
    if (processManager->processTable->runningId == -1)
    {
        return;
    }
    
    int processIdToDelete = processManager->processTable->runningId;
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


void execute(ProcessManager *processManager, int typeOfScheduler)
{
    int processToRunId = nextProcessReady(processManager->processTable->ready, typeOfScheduler);

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


void processExecuting(ProcessManager *processManager, int typeOfScheduler)
{
    if (processManager->processTable->runningId < 0 || processManager->cpu->runningProcess == NULL)
    {
        execute(processManager, typeOfScheduler);
        processUnblock(processManager);
        if (processManager->kill)
        {
            freeProcessManager(processManager);
        }

        return;
    }

    if (processManager->cpu->executing_timer >= processManager->cpu->program_timer)
    {
        scheduleProcess(processManager, typeOfScheduler);
    }
    processUnblock(processManager);
    upperInterpreter(processManager, typeOfScheduler);
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

//ISSO DEVE IR PARA OUTRA TAD----------------------------------------------

typedef struct {
    char** linhas;
    int tamanho;
} Arquivo;

Arquivo lerArquivo(const char* nomeArquivo) {
    const char* caminhoArquivo = "../files/";
    char caminhoCompleto[100];
    strcpy(caminhoCompleto, caminhoArquivo);
    strcat(caminhoCompleto, nomeArquivo);

    FILE* arquivo = fopen(caminhoCompleto, "r");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        Arquivo arq;
        arq.linhas = NULL;
        arq.tamanho = -1;
        return arq;
    }

    // Contar o número de linhas do arquivo
    int numeroLinhas = 0;
    char ch;
    while ((ch = fgetc(arquivo)) != EOF) {
        if (ch == '\n') {
            numeroLinhas++;
        }
    }
    // Se o arquivo não termina com uma nova linha, incrementa o contador
    if (ftell(arquivo) > 0) {
        numeroLinhas++;
    }
    rewind(arquivo);

    // Aloca memória para as linhas
    char** linhas = (char**)malloc(numeroLinhas * sizeof(char*));
    if (linhas == NULL) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        fclose(arquivo);
        Arquivo arq;
        arq.linhas = NULL;
        arq.tamanho = -1;
        return arq;
    }

    // Lê as linhas do arquivo
    char buffer[256];
    int linhaAtual = 0;
    while (fgets(buffer, sizeof(buffer), arquivo) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';  // Remove o caractere de nova linha
        linhas[linhaAtual] = strdup(buffer);   // Aloca memória e copia a linha
        linhaAtual++;
    }

    fclose(arquivo);

    Arquivo arq;
    arq.linhas = linhas;
    arq.tamanho = numeroLinhas;
    return arq;
}

//--------------------------------------------------------------------------

void processRewind(ProcessManager *processManager, char *filename)
{
<<<<<<< Updated upstream:src/manager/process_manager.c
    removeFromProcessTableQueue(processManager->processTable->processTableCellQueue,
                                processManager->processTable->runningId);

    insertToProcessTableQueue(processManager->processTable->processTableCellQueue, filename, -1, processManager->timer);
=======
    //erro no filename
    if (filename == NULL)
    {
        printf("Erro no nome do arquivo\n");
        return;
    }
    Arquivo arq;
    arq = lerArquivo(filename);
    processManager->cpu->runningProcess->program = realloc(processManager->cpu->runningProcess->program, arq.tamanho);
    printf("%s", *arq.linhas);
    printf("%d", arq.tamanho);
    processManager->cpu->runningProcess->program = arq.linhas;
    processManager->cpu->runningProcess->numLines = arq.tamanho;
    processManager->cpu->programCounter = 0;
>>>>>>> Stashed changes:src/manager/processmanager.c
}


void attExec(ProcessManager *processManager)
{
    ProcessTableCell *runningProcessCell = getProcessTableCellByProcessId(
            processManager->processTable->processTableCellQueue,
            processManager->processTable->runningId);

    runningProcessCell->programCounter = processManager->cpu->programCounter;
    runningProcessCell->CPUTime++;
}


void upperInterpreter(ProcessManager *processManager, int typeOfScheduler)
{
    int blockTime;
    int PcPlus;
    char *filename;
    int cpuResp = interpreter(processManager->cpu, &blockTime, &filename, &PcPlus);

    processManager->cpu->programCounter++;
    attExec(processManager);

    switch (cpuResp)
    {
        case 0:
            break;
        case 1://Bloqueia esse processo simulado por n unidades de tempo.
            blockProcess(processManager, blockTime, typeOfScheduler);
            break;
        case 2://Acaba com o processo atual e coloca outro no lugar
            endProcess(processManager, typeOfScheduler);
            break;
        case 3://Crian um novo processo com base no atual do CPU
            processCP(processManager, PcPlus);
            break;
        case 4://Recria o processo atual com base em um arquivo
            processRewind(processManager, filename);
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