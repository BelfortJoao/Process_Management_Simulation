#ifndef SRC_EXECUTINGSTATE_H
#define SRC_EXECUTINGSTATE_H

typedef int RunningProcess; //Guarda o processo que está atualmente rodando no CPU

void contextExchange(int id, RunningProcess *runningProcess);

#endif