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
int nextReady(Ready* r){
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < r->size; ++j) {
            if(r->queues[i][j]!=-1){
                return r->queues[i][j];
            }
        }
    }
    return -1;
}
void insertReady(Ready* r, int pid,int prior) {
    for (int k = 0; k < r->size; ++k) {
        if (r->queues[prior][k] == -1) {
            r->queues[prior][k] = pid;
            return;
        }
        }
    printf("lista de prontos Cheia.");
}

void removeReady(Ready* r, int pid) {
    int i = 0;
    for (int j = 0; j < 4; ++j) {
        for (int k = 0; k < r->size; ++k) {
            if(r->queues[j][k]== pid){
                r->queues[j][k]=-1;
            }
        }
    }
}

//TA TUDO ERRADO AQUI CONCERTO AMANHÃ

void moveFromPriorityToPriority(Ready* ready, int sourcePriority, int destinationPriority, int processId) {
    int* sourceQueue = ready->queues[sourcePriority];
    int* destinationQueue = ready->queues[destinationPriority];

    // Find the process index in the source queue
    int processIndex = -1;
    for (int i = 0; i < ready->size; i++) {
        if (sourceQueue[i] == processId) {
            processIndex = i;
            break;
        }
    }

    // If the process was found in the source queue
    if (processIndex != -1) {
        // Remove it from the source queue
        for (int i = processIndex; i < ready->size - 1; i++) {
            sourceQueue[i] = sourceQueue[i + 1];
        }
        // Add it to the destination queue
        processId;
    }
}
