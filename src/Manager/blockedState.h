//
// Created by belfort on 4/22/23.
//

#ifndef SRC_BLOCKEDSTATE_H
#define SRC_BLOCKEDSTATE_H
typedef int* blockeds;
void initBlockeds(blockeds* b, int size);
void freeBlockeds(blockeds* b);
void insertBlocked(blockeds b, int pid);
void removeBlocked(blockeds b, int pid);
#endif //SRC_BLOCKEDSTATE_H
