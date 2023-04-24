//
// Created by belfort on 4/22/23.
//

#ifndef SRC_EXECUTINGSTATE_H
#define SRC_EXECUTINGSTATE_H
typedef int executing; //Guarda o processo que está atualmente rodando no CPU
void contextExchange(int ID, executing* ex);
#endif //SRC_EXECUTINGSTATE_H
