//
// Created by belfort on 4/22/23.
//

#include <malloc.h>
#include "blockedState.h"
void initBlockeds(blockeds* b, int size) {
    // Allocate memory for the blocked array
    *b = (int*) malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) {
        (*b)[i] = -1; // Initialize all elements to -1
    }
}

void freeBlockeds(blockeds* b) {
    free(*b);
    *b = NULL;
}

void insertBlocked(blockeds b, int pid) {
    int i = 0;
    while (b[i] != -1) {
        i++;
    }
    b[i] = pid;
}

void removeBlocked(blockeds b, int pid) {
    int i = 0;
    while (b[i] != pid && b[i] != -1) {
        i++;
    }
    if (b[i] == pid) {
        while (b[i] != -1) {
            b[i] = b[i + 1];
            i++;
        }
    }
}