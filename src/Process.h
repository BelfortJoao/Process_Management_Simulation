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
void initProcess(process* proc, char* name);
void initMemory(process* proc, int lengthMem);
void excludeProcess(process* proc);
void declareVar(process* proc, int position);
void changeVar(process* proc, int possition, int value);
void addVar(process* proc, int possition, int value);
void subVar(process* proc, int possition, int value);
process* generateNewProcess(process* proc);
#endif //SRC_PROCESS_H
