#ifndef SRC_MANAGER_H
#define SRC_MANAGER_H

#include "cpu.h"
#include "processtable.h"

typedef struct {
    CPU cpu; //Representa o unico núcleo que tem no computador
    ProcessTable processTable; //Tabela de processos do computador
    Timer timer; //Tempo que o computador está ativo
    int kill; //Indica se deve terminar o computador
    int freeID; //Indica qualq é o proximo idArray livre para se criar um novo processo
} Computer;

void initComputer(Computer *comp, char *arq); //inicializa todo o computador
void processExecuting(
        Computer *comp); //Aumenta o tempo em 1 unidade, e executa o CPUSup, escalona, diminui o tempo de bloqueio
void scheduleProcess(Computer *comp); //U checa se o processo atual ja executou todo tempo que tem de cpu
void clockUpPC(Computer *comp);//U sobe o timer de todas as estruturas inferiores;
void killComputer(Computer *comp); //M Termina todo o progama e libera a memoria das estruturas
void uperInterpreter(Computer *comp); //Interpreta os sinais da CPU inferior e executa as funções abaixo
void blockProcess(Computer *comp, int blockT); //(B n)Bloqueia o processo na CPU em N unidades de tempo
void processUnblock(Computer *comp); //Faz o processo sair de Bloqueado para pronto
void processRewind(Computer *comp,
                   char *arq); //(R arq) Le um arquivo e reinicia os status do processo atual da cpu o substituindo pelo arq
void endProcess(Computer *comp); //T termina o processo simulado atual e passa o cpu para o proximo processo pronto
void processCP(Computer *comp, Process *proc,
               int PcPlus); //(F n) cria um novo processo em estado pronto,compiando o atual, e coloca seu programCounterArray na linha pc_atual+n
void execute(Computer *comp);

void attExec(Computer *comp);

#endif
