#ifndef SRC_EXECUTINGSTATE_H
#define SRC_EXECUTINGSTATE_H

typedef int currOnExecution; //Guarda o processo que está atualmente rodando no CPU

void contextExchange(int ID, currOnExecution *executingProcess);

#endif