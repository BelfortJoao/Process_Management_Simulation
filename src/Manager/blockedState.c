//
// Created by belfort on 4/22/23.
//

#include <malloc.h>
#include "blockedState.h"
void initBlockeds(blockeds* b, int size) {
    // Allocate memory for the blocked array
    b->Id = (int*) malloc(size * sizeof(int));
    b->blocktime = (int*) malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) {
        (b->Id)[i] = -1; // Initialize all elements to -1
        (b->blocktime)[i] = -1; // Initialize all elements to -1
    }
}

void freeBlockeds(blockeds* b) {
    free(b->blocktime);
    free(b->Id);
}

void insertBlocked(blockeds* b, int pid,int blocktime) {
    int i = 0;
    while (b->Id[i] != -1) {
        i++;
    }
    b->Id[i] = pid;
    b->blocktime[i] = blocktime;
}

void removeBlocked(blockeds* b, int pid) {
    int i = 0;
    while (b->Id[i] != pid && i< sizeof(b->Id)) {
        i++;
    }
    if (b->Id[i] == pid) {
        while (b->Id[i] != -1 && i+1< sizeof(b->Id)) {
            b->Id[i] = b->Id[i + 1];
            b->blocktime[i]=b->blocktime[i+1];
            i++;
        }
    }
}

void blockDownclock(blockeds* b){
    for (int i = 0; i < sizeof(b->Id); ++i) {
        if(b->Id[i]!=-1){b->blocktime[i]--;}
    }
}