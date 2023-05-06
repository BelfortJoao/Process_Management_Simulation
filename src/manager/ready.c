#include <stdio.h>
#include <stdlib.h>
#include "../error/error.h"

#include "ready.h"

Ready *initializeReady(int size)
{
    Ready *ready = (Ready *) malloc(sizeof(Ready));

    if (!ready)
    {
        printf(ALLOCATION_ERROR, "ready processes");
        return NULL;
    }

    ready->size_at = 0;
    ready->maxSize = size;

    for (int i = 0; i < 4; i++)
    {
        ready->queues[i] = *createQueue();
    }

    return ready;
}

queue *createQueue()
{
    queue *q = (queue *) malloc(sizeof(queue));
    q->front = NULL;
    q->rear = NULL;
    return q;
}

int nextReady(Ready *ready)
{
    for (int i = 0; i < 4; i++)
    {
        if (ready->queues[i].front != NULL)
        {
            int id = ready->queues[i].front->id;
            return id;
        }
    }
    //printEmptyQueue();
    return -1;
}

bool insertToReadyQueue(Ready *ready, int id, int prior)
{
    if (prior > 3)
    {
        return true;
    }
    if (ready->size_at == ready->maxSize)
    {
        return false;
    }
    node *newNode = (node *) malloc(sizeof(node));
    newNode->id = id;
    newNode->next = NULL;
    if (ready->queues[prior].rear == NULL)
    {
        ready->queues[prior].front = newNode;
        ready->queues[prior].rear = newNode;
    } else
    {
        ready->queues[prior].rear->next = newNode;
        ready->queues[prior].rear = newNode;
    }
    ready->size_at++;
    return true;
}

bool removeFromReadyQueue(Ready *ready, int id)
{
    for (int i = 0; i < 4; i++)
    {
        node *aux = ready->queues[i].front;
        node *prev = NULL;
        while (aux != NULL)
        {
            if (aux->id == id)
            {
                if (prev == NULL)
                {
                    ready->queues[i].front = aux->next;
                } else
                {
                    prev->next = aux->next;
                }
                if (aux->next == NULL)
                {
                    ready->queues[i].rear = prev;
                }
                free(aux);
                ready->size_at--;
                return true;
            }
            prev = aux;
            aux = aux->next;
        }
    }
    return false;
}

void printState(Ready *rs)
{
    printf("\n+-----------------+\n");
    printf("| Estado da fila: |\n");
    printf("+-----------------+\n");
    for (int i = 0; i < 4; i++)
    {
        printf("| Fila %d: ", i);
        if (rs->queues[i].front == NULL)
        {
            printf("Vazia");
        } else
        {
            node *atual = rs->queues[i].front;
            while (atual != NULL)
            {
                printf("%d ", atual->id);
                atual = atual->next;
            }
        }
        printf("\n");
    }
    printf("\n");
}

void freeReady(Ready *ready)
{
    for (int i = 0; i < 4; i++)
    {
        node *aux = ready->queues[i].front;
        while (aux != NULL)
        {
            node *next = aux->next;
            free(aux);
            aux = next;
        }
    }
}