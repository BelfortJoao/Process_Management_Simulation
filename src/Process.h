//
// Created by belfort on 4/22/23.
//

#ifndef SRC_PROCESS_H
#define SRC_PROCESS_H

#include <bits/types/FILE.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef int* memory;
typedef char** program;
typedef struct process{
    memory mem;
    program prog;
    int numLines;
    int lengthMem;
}process;
void initProcess(struct process* proc, char* name);
void initMemory(struct process* proc);
void excludeProcess(struct process* proc);
#endif //SRC_PROCESS_H
