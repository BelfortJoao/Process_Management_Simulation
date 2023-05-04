//
// Created by matheuspeixoto on 04/05/23.
//
#include "readystate.h"

void initReady(Ready *ready, int maxSize)
{

    ready->maxSize = maxSize;
    for(int i = 0; i < 4; i++){
        ready->queues[i] = *createQueue();
    }
}

queue* createQueue()
{
    queue* fila;
    fila->front = (Apontador)malloc(sizeof(fila));
    fila->rear = fila->front;
    fila->front->No.next = NULL;

    return fila;
}
void insertReady(Ready *rs, int id, int prior)
{
    // Verifica se a fila n é válida
    if (prior < 0 || prior > 3) {
        prior=3;
        return;
    }
    printf("inserindo %d na fila %d",id, prior);
    // Verifica se há espaço disponível na fila
    if (rs->size_at >= rs->maxSize) {
        printf("Não há espaço disponível na fila %d.\n", prior);
        return;
    }

    // Cria um novo nó com o ID do item a ser adicionado
    node* novoNo;
    novoNo = (node*)malloc(sizeof(node));
    if(novoNo == NULL) return;

    if(rs->queues[prior].front == NULL)
        rs->queues[prior].front = novoNo;
    else
        rs->queues[prior].rear->No.next = novoNo;
    rs->queues[prior].rear = novoNo;

    rs->size_at++;

}

void removeReady(Ready* rs, int id) {
    int i;
    for (i = 0; i < 4; i++) {
        node* atual = rs->queues[i].front;
        node* anterior = NULL;
        while (atual != NULL) {
            if (atual->id == id) {
                // Remove o nó da fila
                if (anterior == NULL) {
                    rs->queues[i].front = atual->next;
                } else {
                    anterior->next = atual->next;
                }
                if (atual == rs->queues[i].rear) {
                    rs->queues[i].rear = anterior;
                }
                free(atual);

                // Atualiza o tamanho da estrutura readystate
                rs->size_at--;

                return;
            }
            anterior = atual;
            atual = atual->next;
        }
    }
    printf("ID não encontrado em nenhuma das filas!\n");
}

int nextReady(Ready* rs) {
    int i;
    for (i = 0; i < 4; i++) {
        if (rs->queues[i].front != NULL) {
            return rs->queues[i].front->No.id;
        }
    }
    return -1; // Nenhum ID encontrado
}

void printState(Ready* rs) {
    int i;
    for (i = 0; i < 4; i++) {
        printf("Fila %d: ", i);
        if (rs->queues[i].front == NULL) {
            printf("Vazia\n");
        } else {
            node* atual = rs->queues[i].front;
            while (atual != NULL) {
                printf("%d ", atual->id);
                atual = atual->next;
            }
            printf("\n");
        }
    }
}

void freeReady(Ready* rs) {
    int i, j;
    for (i = 0; i < 4; i++) {
        node* atual = rs->queues[i].front;
        while (atual != NULL) {
            node* proximo = atual->next;
            free(atual);
            atual = proximo;
        }
        rs->queues[i].front = NULL;
        rs->queues[i].rear = NULL;
    }
    free(rs);
}
