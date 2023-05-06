#ifndef SRC_PROCESSMANAGER_H
#define SRC_PROCESSMANAGER_H

#include "cpu.h"
#include "processtable.h"

typedef struct
{
    CPU *cpu; //Representa o unico núcleo que tem no computador
    ProcessTable *processTable; //Tabela de processos do computador
    Timer timer; //Tempo que o computador está ativo
    int kill; //Indica se deve terminar o computador
    int freeID; //Indica qualq é o proximo idArray livre para se criar um novo processo
} ProcessManager;

ProcessManager *initializeProcessManager();

ProcessManager *initializeProcessManagerFromFile(char *filename);

void processExecuting(ProcessManager *processManager);

void scheduleProcess(ProcessManager *processManager);

void clockUpPC(ProcessManager *processManager);

void freeProcessManager(ProcessManager *processManager);

void upperInterpreter(ProcessManager *processManager);

void blockProcess(ProcessManager *processManager, int blockTime);

void processUnblock(ProcessManager *processManager);

void processRewind(ProcessManager *processManager, char *filename);

void endProcess(ProcessManager *processManager);

void processCP(ProcessManager *processManager, Process *process, int PcPlus);

void execute(ProcessManager *processManager);

void attExec(ProcessManager *processManager);

#endif