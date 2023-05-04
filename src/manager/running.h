#ifndef SRC_RUNNING_H
#define SRC_RUNNING_H

typedef int Running; //Guarda o processo que está atualmente rodando no CPU

Running *initializeRunningProcess();

void contextExchange(int id, Running *runningProcess);

#endif