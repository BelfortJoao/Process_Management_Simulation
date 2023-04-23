//
// Created by belfort on 4/22/23.
//

#include <malloc.h>
#include "ReadyState.h"



void initReady(Ready* r, int size) {
    // Allocate memory for the ready queues
    r->queues = (int**) malloc(4 * sizeof(int*));
    for (int i = 0; i < 4; i++) {
        r->queues[i] = (int*) malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) {
            r->queues[i][j] = -1; // Initialize all elements to -1
        }
    }
    r->size = size;
}

void freeReady(Ready* r) {
    for (int i = 0; i < 4; i++) {
        free(r->queues[i]);
    }
    free(r->queues);
    r->queues = NULL;
    r->size = 0;
}

void insertReady(Ready* r, int pid, int priority) {
    int i = 0;
    while (r->queues[priority][i] != -1 && i < r->size) {
        i++;
    }
    if (i < r->size) {
        r->queues[priority][i] = pid;
    }
}

void removeReady(Ready* r, int pid, int priority) {
    int i = 0;
    while (r->queues[priority][i] != pid && i < r->size) {
        i++;
    }
    if (i < r->size) {
        while (i < r->size - 1 && r->queues[priority][i + 1] != -1) {
            r->queues[priority][i] = r->queues[priority][i + 1];
            i++;
        }
        r->queues[priority][i] = -1;
    }
}

//TA TUDO ERRADO AQUI CONCERTO AMANHÃ
void moveFromPriorityToPriority(Ready* ready, int sourcePriority, int destinationPriority, int processId) {
    int* sourceQueue = ready->queues[sourcePriority];
    int* destinationQueue = ready->queues[destinationPriority];

    // Find the process index in the source queue
    int processIndex = -1;
    for (int i = 0; i < sourceSize; i++) {
        if (sourceQueue[i] == processId) {
            processIndex = i;
            break;
        }
    }

    // If the process was found in the source queue
    if (processIndex != -1) {
        // Remove it from the source queue
        for (int i = processIndex; i < sourceSize - 1; i++) {
            sourceQueue[i] = sourceQueue[i + 1];
        }
        // Add it to the destination queue
        processId;
    }
}