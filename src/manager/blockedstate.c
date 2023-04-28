#include <malloc.h>

#include "blockedstate.h"

void initBlockedIds(BlockedIds *b, int size) {
    // Allocate Memory for the blockedArray array
    b->id = (int *) malloc(size * sizeof(int));
    b->blockTime = (int *) malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) {
        (b->id)[i] = -1; // Initialize all elements to -1
        (b->blockTime)[i] = -1; // Initialize all elements to -1
    }

    //print all elements
    for (int i = 0; i < size; i++) {
        printf("%d ", (b->id)[i]);
    }

}

void freeBlockedIds(BlockedIds *b) {
    free(b->blockTime);
    free(b->id);
}

void insertBlockedId(BlockedIds *b, int pid, int blocktime) {
    int i = 0;
    while (b->id[i] != -1) {
        i++;
    }
    b->id[i] = pid;
    b->blockTime[i] = blocktime;
}

void removeBlockedId(BlockedIds *b, int pid) {
    int i = 0;
    while (b->id[i] != pid && i < sizeof(b->id)) {
        i++;
    }
    if (b->id[i] == pid) {
        while (b->id[i] != -1 && i + 1 < sizeof(b->id)) {
            b->id[i] = b->id[i + 1];
            b->blockTime[i] = b->blockTime[i + 1];
            i++;
        }
    }
}

void blockDownClock(BlockedIds *b) {
    for (int i = 0; i < sizeof(b->id); ++i) {
        if (b->id[i] != -1) { b->blockTime[i]--; }
    }
}