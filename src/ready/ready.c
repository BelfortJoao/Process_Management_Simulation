#include <stdio.h>
#include <stdlib.h>
#include "../error/error.h"

#include "ready.h"

#define DEFAULT_QUEUE_SIZE 4


Ready *initializeReady(int size)
{
    Ready *ready = (Ready *) malloc(sizeof(Ready));

    if (!ready)
    {
        printf(ALLOCATION_ERROR, "ready processes");
        return NULL;
    }

    ready->size = 0;
    ready->maxSize = size;

    ready->queues = (Queue **) malloc(DEFAULT_QUEUE_SIZE * sizeof(Queue *));

    if (!ready->queues)
    {
        printf(ALLOCATION_ERROR, "array of queues");
        return NULL;
    }

    for (int i = 0; i < DEFAULT_QUEUE_SIZE; i++)
    {
        ready->queues[i] = initializeQueue();
    }

    return ready;
}


int nextProcessReady(Ready *ready, int typeOfScheduler)
{
    if (typeOfScheduler == 1){
        for (int i = 0; i < DEFAULT_QUEUE_SIZE; i++)
        {
            if (ready->queues[i]->front)
            {
                return ready->queues[i]->front->id;
            }
        }
    }

    else if (typeOfScheduler == 2){
        int filaSelecionada = -1;
        int tamanhoFila = 0;

        // Encontra a primeira fila não vazia e obtém o tamanho
        for (int i = 0; i < DEFAULT_QUEUE_SIZE; i++) {
            if (ready->queues[i]->front) {
                filaSelecionada = i;
                break;
            }
        }

        if (filaSelecionada != -1) {
            // Calcula o tamanho da fila selecionada
            QueueNode *id = ready->queues[filaSelecionada]->front;
            while (id != NULL) {
                tamanhoFila++;
                id = id->next;
            }

            if (tamanhoFila > 0) {
                // Gera um índice aleatório com base no tamanho da fila
                int indiceAleatorio = rand() % tamanhoFila;

                // Localiza o ID do processo no índice aleatório da fila selecionada
                QueueNode *idSelecionado = ready->queues[filaSelecionada]->front;
                for (int i = 0; i < indiceAleatorio; i++) {
                    idSelecionado = idSelecionado->next;
                }

                return idSelecionado->id;
            }
        }

    }
    return -1;
}


bool insertToReady(Ready *ready, int id, int priority)
{
    if (priority > 3)
    {
        return true;
    }

    if (ready->size == ready->maxSize)
    {
        return false;
    }

    if (insertToQueue(ready->queues[priority], id))
    {
        ready->size++;
        return true;
    }

    return false;
}


bool removeFromReady(Ready *ready, int id)
{
    for (int i = 0; i < DEFAULT_QUEUE_SIZE; i++)
    {
        Queue *currQueue = ready->queues[i];

        if (removeFromQueue(currQueue, id))
        {
            ready->size--;
            return true;
        }
    }

    return false;
}


void freeReady(Ready *ready)
{
    if (ready)
    {
        for (int i = 0; i < DEFAULT_QUEUE_SIZE; i++)
        {
            freeQueue(ready->queues[i]);
        }

        if (ready->queues)
        {
            free(ready->queues);
        }

        free(ready);
    }
}