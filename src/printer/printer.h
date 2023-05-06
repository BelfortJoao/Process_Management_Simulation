#ifndef SRC_PRINTER_H
#define SRC_PRINTER_H

#include "../manager/processtable.h"
#include "../manager/readystate.h"

typedef struct {
    int **responseTimes;
    int size;
} Print;

void printMedResponseTime(Print *print);

void printProcessTable(ProcessTable *processTable);

void printInitMem(char *arg1);

void printDeclareVar(char *arg1);

void printChangeVar(char *arg1, char *arg2);

void printAddVar(char *arg1, char *arg2);

void printSubVar(char *arg1, char *arg2);

void printBlkProcess(char *arg1);

void printTerProcess();

void printCopyProcess();

void printReadFile(char *arg1);

void printState(Ready* ready);

//void printFullQueue();

//void printProcessNotFound();

//void printEmptyQueue()

#endif