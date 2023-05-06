#include <stdio.h>
#include <stdlib.h>
#include "../error/error.h"

#include "printer.h"


Printer *initializePrinter(int size)
{
    Printer *printer = (Printer *) malloc(sizeof(Printer));

    if (printer == NULL)
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

void printInitMem(int initialMemorySize)
{
    printf("Memory initialized with size %d.\n", initialMemorySize);
}

void printDeclareVar(int position)
{
    printf("Memory at position %d has been cleared.\n", position);
}

void printChangeVar(int position, int value)
{
    printf("Memory changed to %d at position %d.\n", value, position);
}

void printAddVar(int position, int increaseValue)
{
    printf("Memory increased by %d at position %d.\n", increaseValue, position);
}

void printSubVar(int position, int decreaseValue)
{
    printf("Posição de memória %d subtraida em %d\n", decreaseValue, position);
}

void printBlkProcess(int timeUnits)
{
    printf("Blocking process for %d time unit(s).\n", timeUnits);
}

void printTerProcess()
{
    printf("Terminando processo\n");
}

void printCopyProcess()
{
    printf("Copiando processo\n");
}

void printReadFile(char *arg1)
{
    printf("Lendo o arquivo %s\n", arg1);
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

void printFullQueue()
{
    printf("Fila cheia\n");
}

void printProcessNotFound()
{
    printf("Processo não encontrado\n");
}

void printEmptyQueue()
{
    printf("Fila vazia\n");
}

void printFinishExe()
{
    printf("Não ha mais nada para executar\n");
}