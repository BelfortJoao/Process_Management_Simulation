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

ProcessManager *initializeProcessManagerFromFile(char *filename); //inicializa todo o computador
void processExecuting(
        ProcessManager *processManager); //Aumenta o tempo em 1 unidade, e executa o CPUSup, escalona, diminui o tempo de bloqueio
void scheduleProcess(ProcessManager *processManager); //U checa se o processo atual ja executou todo tempo que tem de cpu
void clockUpPC(ProcessManager *processManager);//U sobe o timer de todas as estruturas inferiores;
void killComputer(ProcessManager *processManager); //M Termina todo o progama e libera a memoria das estruturas
void uperInterpreter(ProcessManager *processManager); //Interpreta os sinais da CPU inferior e executa as funções abaixo
void blockProcess(ProcessManager *processManager, int blockTime); //(B n)Bloqueia o processo na CPU em N unidades de tempo
void processUnblock(ProcessManager *processManager); //Faz o processo sair de Bloqueado para pronto
void processRewind(ProcessManager *processManager,
                   char *filename); //(R filename) Le um arquivo e reinicia os status do processo atual da cpu o substituindo pelo filename
void
endProcess(ProcessManager *processManager); //T termina o processo simulado atual e passa o cpu para o proximo processo pronto
void processCP(ProcessManager *processManager, Process *process,
               int PcPlus); //(F n) cria um novo processo em estado pronto,compiando o atual, e coloca seu programCounterArray na linha pc_atual+n
void execute(ProcessManager *processManager);

void attExec(ProcessManager *processManager);

#endif
