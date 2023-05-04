//
// Created by matheuspeixoto on 04/05/23.
//

#ifndef SRC_READYSTATE_H
#define SRC_READYSTATE_H

#include <stdio.h>
#include <stdlib.h>

/*
typedef struct {
    int id;
    struct node* next;
} node;
typedef struct Ready *Apontador;
typedef struct {
    Apontador front;
    Apontador rear;
} queue;

typedef struct Ready{
    int size_at;
    int maxSize;
    queue queues[4];
}Ready;
*/
typedef struct node{
    int id;
    struct node* next;
}node;

typedef struct Ready* Apontador;

typedef struct {
    Apontador front;
    Apontador rear;
} queue;


typedef struct Ready{
    int size_at, maxSize;
    node No;
    queue queues[4];
}Ready;



//inicializar
//void initReady(Ready *ready, int size)
void initReady(Ready *ready, int maxSize);
queue* createQueue();
void insertReady(Ready* rs,int id, int prior);
//free
void freeReady(Ready *ready);
//pegar id do proximo
int nextReady(Ready *ready);
//remover
void removeReady(Ready *ready, int processId);
//print
void printState( Ready *ready);
#endif