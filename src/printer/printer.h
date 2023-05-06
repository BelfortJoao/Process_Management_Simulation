#ifndef SRC_PRINTER_H
#define SRC_PRINTER_H

#include "../manager/processtable.h"
#include "../manager/ready.h"

typedef struct
{
    int **responseTimes;
    int size;
} Printer;

Printer *initializePrinter(int size);

void printAverageResponseTime(Printer *printer);

void printProcessTable(ProcessTable *processTable);

void printInitMem(int memoryInitialSize);

void printDeclareVar(int position);

void printChangeVar(int position, int value);

void printAddVar(int position, int increaseValue);

void printSubVar(int position, int decreaseValue);

void printBlkProcess(int timeUnits);

void printTerProcess();

void printCopyProcess();

void printReadFile(char *arg1);

void printState(Ready *ready);

void printFullQueue();

void printProcessNotFound();

void printEmptyQueue();

void printFinishExe();

#endif