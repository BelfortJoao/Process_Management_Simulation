#ifndef SRC_READYSTATE_H
#define SRC_READYSTATE_H


typedef struct node {
    int id;
    struct node* next;
} node;

typedef struct {
    node* front;
    node* rear;
} queue;

typedef struct {
    int size_at;
    int maxSize;
    queue queues[4];
} Ready;

void initReady(Ready *ready, int maxSize);
queue* createQueue();
int insertReady(Ready *ready, int id, int prior);
void removeReady(Ready *ready, int id);
int nextReady(Ready *ready);
void freeReady(Ready *ready);

#endif