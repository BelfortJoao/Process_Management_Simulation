//
// Created by belfort on 4/22/23.
//

#include <malloc.h>
#include "readystate.h"


void initReady(Ready *ready, int maxSize){
    ready->size_at = 0;
    ready->maxSize= maxSize;
    for (int i = 0; i < 4; i++) {
        ready->queues[i] = *createQueue(); // Inicializa as filas FIFO com tamanho zero e ponteiros para NULL
    }

}

queue* createQueue() {
    queue* q = (queue*) malloc(sizeof(queue));
    q->front = NULL;
    q->rear = NULL;
    return q;
}

int insertReady(Ready *ready, int id, int prior) {
    if (prior > 3){
        prior = 3;
        return 0;
    }
    if (ready->size_at == ready->maxSize) {
        //printFullQueue();
        return 1;
    }
    node* newNode = (node*) malloc(sizeof(node));
    newNode->id = id;
    newNode->next = NULL;
    if (ready->queues[prior].rear == NULL) {
        ready->queues[prior].front = newNode;
        ready->queues[prior].rear = newNode;
    } else {
        ready->queues[prior].rear->next = newNode;
        ready->queues[prior].rear = newNode;
    }
    ready->size_at++;
    return 0;
}

void removeReady(Ready *ready, int id) {
    for (int i = 0; i < 4; i++) {
        node* aux = ready->queues[i].front;
        node* prev = NULL;
        while (aux != NULL) {
            if (aux->id == id) {
                if (prev == NULL) {
                    ready->queues[i].front = aux->next;
                } else {
                    prev->next = aux->next;
                }
                if (aux->next == NULL) {
                    ready->queues[i].rear = prev;
                }
                free(aux);
                ready->size_at--;
                return;
            }
            prev = aux;
            aux = aux->next;
        }
    }
    //printProcessNotFound();
}

int nextReady(Ready *ready) {
    for (int i = 0; i < 4; i++) {
        if (ready->queues[i].front != NULL) {
            int id = ready->queues[i].front->id;
            return id;
        }
    }
    //printEmptyQueue();
    return -1;
}

void freeReady(Ready *ready) {
    for (int i = 0; i < 4; i++) {
        node* aux = ready->queues[i].front;
        while (aux != NULL) {
            node* next = aux->next;
            free(aux);
            aux = next;
        }
    }
}