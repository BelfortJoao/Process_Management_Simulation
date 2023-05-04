#ifndef SRC_BLOCKED_H
#define SRC_BLOCKED_H

typedef struct
{
    int *blockTimes;
    int *ids;
} Blocked; // Processos que estão atualmente bloqueados


Blocked *initializeBlocked(int size);

void insertBlockedId(Blocked *blocked, int processId, int blockTime);

void removeBlockedId(Blocked *blocked, int processId);

void blockDownClock(Blocked *blocked);

void freeBlocked(Blocked *blocked);

#endif