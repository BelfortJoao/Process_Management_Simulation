#ifndef SRC_BLOCKEDSTATE_H
#define SRC_BLOCKEDSTATE_H

typedef struct
{
    int *blockTime;
    int *id;
} BlockedProcesses; // Processos que estão atualmente bloqueados


BlockedProcesses *initializeBlockedProcesses(int size);

void freeBlockedIds(BlockedProcesses *b);

void insertBlockedId(BlockedProcesses *b, int pid, int blockTime);

void removeBlockedId(BlockedProcesses *b, int pid);

void blockDownClock(BlockedProcesses *b);

#endif