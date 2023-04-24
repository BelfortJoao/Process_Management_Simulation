//
// Created by belfort on 4/22/23.
//

#ifndef SRC_BLOCKEDSTATE_H
#define SRC_BLOCKEDSTATE_H

typedef struct blockeds{
    int* blocktime;
    int* Id;
}blockeds; //Processos que estão atualmente bloqueados
void initBlockeds(blockeds* b, int size);
void freeBlockeds(blockeds* b);
void insertBlocked(blockeds* b, int pid, int blocktime);
void removeBlocked(blockeds* b, int pid);
void blockDownclock(blockeds* b);
#endif //SRC_BLOCKEDSTATE_H
