//
// Created by belfort on 4/23/23.
//

#ifndef SRC_MANAGER_H
#define SRC_MANAGER_H

#include "cpu.h"
#include "ProcessTable.h"
#include <limits.h>
#include <math.h>
typedef struct computer{
    CPU cpu; //Representa o unico núcleo que tem no computador
    processTable proctb; //Tabela de processos do computador
    timer clock; //Tempo que o computador está ativo
    int kill; //Indica se deve terminar o computador
    int freeID; //Indica qualq é o proximo ID livre para se criar um novo processo
}computer;

void initComputer(computer* comp, char* arq); //inicializa todo o computador
void processExecuting(computer* comp); //Aumenta o tempo em 1 unidade, e executa o CPUSup, escalona, diminui o tempo de bloqueio
void processEscalonating(computer* comp); //U checa se o processo atual ja executou todo tempo que tem de cpu
void clockUpPC(computer* comp);//U sobe o clock de todas as estruturas inferiores;
void computerKill(computer* comp); //M Termina todo o progama e libera a memoria das estruturas
void uperInterpreter(computer* comp); //Interpreta os sinais da CPU inferior e executa as funções abaixo
void processBlock(computer* comp, int blockT); //(B n)Bloqueia o processo na CPU em N unidades de tempo
void processUnblock(computer* comp); //Faz o processo sair de Bloqueado para pronto
void processRewind(computer* comp,char* arq); //(R arq) Le um arquivo e reinicia os status do processo atual da cpu o substituindo pelo arq
void processExterminate(computer* comp); //T termina o processo simulado atual e passa o cpu para o proximo processo pronto
void processCP(computer* comp, process* proc, int PcPlus); //(F n) cria um novo processo em estado pronto,compiando o atual, e coloca seu pc na linha pc_atual+n
void execute(computer* comp);
void attExec(computer* comp);
#endif //SRC_MANAGER_H
