#ifndef SRC_BLOCKEDSTATE_H
#define SRC_BLOCKEDSTATE_H

typedef struct {
    int *blockTime;
    int *id;
} BlockedIds; // Processos que estão atualmente bloqueados


void initBlockedIds(BlockedIds *b, int size);

void freeBlockedIds(BlockedIds *b);

void insertBlockedId(BlockedIds *b, int pid, int blocktime);

void removeBlockedId(BlockedIds *b, int pid);

void blockDownClock(BlockedIds *b);

#endif