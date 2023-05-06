#include <stdio.h>
#include <stdlib.h>
#include "../error/error.h"

#include "printer.h"


Printer *initializePrinter(int size)
{
    Printer *printer = (Printer *) malloc(sizeof(Printer));

    if (!printer)
    {
        printf(ALLOCATION_ERROR, "printer");
        return NULL;
    }

    printer->size = size;

    return printer;
}


void printAverageResponseTime(Printer *printer)
{
    printf("\nTODO: Average response time. Good luck Alan.\n");
}


void printProcessTable(ProcessTable *processTable)
{
    printf("Proximo idArray Livre: %d\n", processTable->nextFreeId);
    printf("Numero atual de processos: %d\n", processTable->tableSize);
    printf("Capacidade da tabela: %d\n", processTable->tableCapacity);
    printf("\n+------------+------------+------------+------------+------------+------------+------------+\n");
    printf("| %-10s | %-10s | %-10s | %-10s | %-10s | %-10s | %-10s |\n", "idArray", "PC", "Pai", "Prioridade",
           "Estado", "Inicio", "T de uso");
    printf("+------------+------------+------------+------------+------------+------------+------------+");
    for (int i = 0; i < processTable->tableCapacity; ++i)
    {
        if (processTable->emptyArray[i] != 0)
        {
            if (i != 0)
            {
                printf("\n|            |            |            |            |            |            |            |");
            }
            printf("\n| %-10d | %-10d | %-10d | %-10d | %-10s | %-10d | %-10d |", processTable->idArray[i],
                   processTable->programCounterArray[i],
                   processTable->parentProcessArray[i], processTable->priorityIdArray[i],
                   processTable->processStateArray[i], processTable->initialTimerArray[i],
                   processTable->CPUTimerArray[i]);
        };
    }
    printf("\n+------------+------------+------------+------------+------------+------------+------------+\n");
}


void printInitialMemorySize(int initialMemorySize)
{
    printf("Memory initialized with size %d.\n", initialMemorySize);
}


void printClearMemory(int position)
{
    printf("Memory at position %d has been cleared.\n", position);
}


void printMemoryChange(int position, int value)
{
    printf("Memory changed to %d at position %d.\n", value, position);
}


void printMemoryIncrease(int position, int increaseValue)
{
    printf("Memory increased by %d at position %d.\n", increaseValue, position);
}


void printDecreaseMemory(int position, int decreaseValue)
{
    printf("Memory decreased by %d at position %d.\n", decreaseValue, position);
}

void printBlockedProcess(int timeUnits)
{
    printf("Blocking process for %d time unit(s).\n", timeUnits);
}


void printFinishProcess()
{
    printf("Finishing process.\n");
}


void printCopyProcess()
{
    printf("Copying process.\n");
}


void printReadFile(char *arg1)
{
    printf("Reading file %s.\n", arg1);
}


void printState(Ready *ready)
{
    printf("\n+-----------------+\n");
    printf("| Estado da fila: |\n");
    printf("+-----------------+\n");

    for (int i = 0; i < 4; i++)
    {
        printf("| Fila %d: ", i);

        if (!ready->queues[i]->front)
        {
            printf("Vazia");
        }
        else
        {
            QueueNode *currQueueNode = ready->queues[i]->front;

            while (currQueueNode)
            {
                printf("%d ", currQueueNode->id);
                currQueueNode = currQueueNode->next;
            }
        }

        printf("\n");
    }

    printf("\n");
}


void printFullQueue()
{
    printf("Queue is full.\n");
}


void printProcessNotFound()
{
    printf("Process not found.\n");
}


void printEmptyQueue()
{
    printf("Queue is empty.\n");
}


void printFinishExe()
{
    printf("Nothing else to run.\n");
}