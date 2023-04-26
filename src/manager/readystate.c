//
// Created by belfort on 4/22/23.
//

#include <malloc.h>
#include "readystate.h"


void initReady(Ready *ready, int size) {
    // Allocate Memory for the readyArray queues
    ready->queues = (int **) malloc(4 * sizeof(int *));
    for (int i = 0; i < 4; i++) {
        ready->queues[i] = (int *) malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) {
            ready->queues[i][j] = -1; // Initialize all elements to -1
        }
    }
    ready->size = size;
}

void freeReady(Ready *ready) {
    for (int i = 0; i < 4; i++) {
        free(ready->queues[i]);
    }
    free(ready->queues);
    ready->queues = NULL;
    ready->size = 0;
}

int nextReady(Ready *ready) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < ready->size; ++j) {
            if (ready->queues[i][j] != -1) {
                return ready->queues[i][j];
            }
        }
    }
    return -1;
}

void insertReady(Ready *ready, int processId, int prior) {
    for (int k = 0; k < ready->size; ++k) {
        if (ready->queues[prior][k] == -1) {
            ready->queues[prior][k] = processId;
            return;
        }
    }
    printf("lista de prontos Cheia.");
}

void removeReady(Ready *ready, int processId) {
    int i = 0;
    for (int j = 0; j < 4; ++j) {
        for (int k = 0; k < ready->size; ++k) {
            if (ready->queues[j][k] == processId) {
                ready->queues[j][k] = -1;
            }
        }
    }
}

//TA TUDO ERRADO AQUI CONCERTO AMANHÃ

void moveFromPriorityToPriority(Ready *ready, int sourcePriority, int destinationPriority, int processId) {
    int *sourceQueue = ready->queues[sourcePriority];
    int *destinationQueue = ready->queues[destinationPriority];

    // Find the Process index in the source queue
    int processIndex = -1;
    for (int i = 0; i < ready->size; i++) {
        if (sourceQueue[i] == processId) {
            processIndex = i;
            break;
        }
    }

    // If the Process was found in the source queue
    if (processIndex != -1) {
        // Remove it from the source queue
        for (int i = processIndex; i < ready->size - 1; i++) {
            sourceQueue[i] = sourceQueue[i + 1];
        }
        // Add it to the destination queue
        processId;
    }
}
