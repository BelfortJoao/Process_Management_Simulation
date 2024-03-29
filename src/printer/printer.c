#include <stdio.h>
#include <stdlib.h>
#include "../error/error.h"

#include "printer.h"
#include "../colour/colour.h"


Printer *initializePrinter(int size) {
    Printer *printer = (Printer *) malloc(sizeof(Printer));

    if (!printer) {
        printf(ALLOCATION_ERROR, "printer");
        return NULL;
    }

    printer->size = size;

    return printer;
}


void printAverageResponseTime(double averageResponseTime)
{
    if(averageResponseTime > 0)
    {
        printf("\nAverage Response Time: %f\n", averageResponseTime);
    }
    else
    {
        printf("No finished processes\n");
    }
}


char *getStateString(enum ProcessState processState) {
    switch (processState) {
        case RUNNING:
            return "RUNNING";
        case READY:
            return "READY";
        case BLOCKED:
            return "BLOCKED";
        default:
            return "ERROR";
    }
}


void printProcessTable(ProcessTable *processTable) {
    printf("Current number of processes: %d.\n", processTable->nextFreeId);
    printf("\n+------------+------------+------------+------------+------------+------------+------------+\n");
    printf("|%s                                    Process Table:                                        %s|\n", B_BLACK, RESET);
    printf("+------------+------------+------------+------------+------------+------------+------------+\n");
    printf("|%s %-10s | %-10s | %-10s | %-10s | %-10s | %-10s | %-10s %s|\n", B_BLACK, "idArray", "PC", "Parent",
           "Priority",
           "State", "Start", "Time", RESET);
    printf("+------------+------------+------------+------------+------------+------------+------------+");

    ProcessTableCellNode *currNode = processTable->processTableCellQueue->front;

    while (currNode) {
        if (currNode->processTableCell->state == RUNNING) {
            printf("\n|%s%s %-10d | %-10d | %-10d | %-10d | %-10s | %-10d | %-10d %s|", B_BLACK, GREEN,
                   currNode->processTableCell->id,
                   currNode->processTableCell->programCounter,
                   currNode->processTableCell->parentProcessId,
                   currNode->processTableCell->priority,
                   getStateString(currNode->processTableCell->state),
                   currNode->processTableCell->initialTime,
                   currNode->processTableCell->CPUTime, RESET);
        } else if (currNode->processTableCell->state == BLOCKED) {
            printf("\n|%s%s %-10d | %-10d | %-10d | %-10d | %-10s | %-10d | %-10d %s|", B_BLACK, RED,
                   currNode->processTableCell->id,
                   currNode->processTableCell->programCounter,
                   currNode->processTableCell->parentProcessId,
                   currNode->processTableCell->priority,
                   getStateString(currNode->processTableCell->state),
                   currNode->processTableCell->initialTime,
                   currNode->processTableCell->CPUTime, RESET);
        } else {
            printf("\n|%s%s %-10d | %-10d | %-10d | %-10d | %-10s | %-10d | %-10d %s|", B_BLACK, CYAN,
                   currNode->processTableCell->id,
                   currNode->processTableCell->programCounter,
                   currNode->processTableCell->parentProcessId,
                   currNode->processTableCell->priority,
                   getStateString(currNode->processTableCell->state),
                   currNode->processTableCell->initialTime,
                   currNode->processTableCell->CPUTime, RESET);
        }

        currNode = currNode->next;
    }
    printf("\n+------------+------------+------------+------------+------------+------------+------------+\n");
}


void printInitialMemorySize(int initialMemorySize) {
    printf("Memory initialized with nextFreeId %d.\n", initialMemorySize);
}


void printClearMemory(int position) {
    printf("Memory at position %d has been cleared.\n", position);
}


void printMemoryChange(int position, int value) {
    printf("Memory changed to %d at position %d.\n", value, position);
}


void printMemoryIncrease(int position, int increaseValue) {
    printf("Memory increased by %d at position %d.\n", increaseValue, position);
}


void printDecreaseMemory(int position, int decreaseValue) {
    printf("Memory decreased by %d at position %d.\n", decreaseValue, position);
}

void printBlockedProcess(int timeUnits) {
    printf("Blocking process for %d time unit(s).\n", timeUnits);
}


void printFinishProcess() {
    printf("Finishing process.\n");
}


void printCopyProcess() {
    printf("Copying process.\n");
}


void printReadFile(char *arg1) {
    printf("Reading file %s.\n", arg1);
}


void printState(Ready *ready) {
    printf("\n+----------------+\n");
    printf("| %sReady Queue states:  %s|\n", B_BLACK, RESET);
    printf("+----------------+\n");

    for (int i = 0; i < 4; i++) {
        printf("| %sQueue %d:%s ", B_BLACK, i, RESET);

        if (!ready->queues[i]->front) {
            printf("%sEMPTY%s", RED, RESET);

        } else {
            QueueNode *currQueueNode = ready->queues[i]->front;

            while (currQueueNode) {

                printf("%s%s%-4d%s", BOLD, CYAN, currQueueNode->id, RESET);

                currQueueNode = currQueueNode->next;
            }
        }

        printf("\n");
    }

    printf("+----------------+\n");
}

void printBlocked(BlockedQueue *blocked) {
    printf("\n+----------------+\n");
    printf("| %sBlocked Queue:  %s|\n", B_BLACK, RESET);
    printf("+----------------+\n");

    if (!blocked->front) {
        printf("| %sEMPTY%s         |", RED, RESET);
    } else {
        QueueNode *currQueueNode = blocked->front;

        while (currQueueNode) {

            printf("%s%s%-4d%s", BOLD, RED, currQueueNode->id, RESET);

            currQueueNode = currQueueNode->next;
        }
    }

    printf("\n+----------------+\n");
}



void printFullQueue() {

    printf("Queue is full.\n");
}


void printProcessNotFound() {
    printf("Process not found.\n");
}


void printEmptyQueue() {
    printf("Queue is empty.\n");
}


void printFinishExe() {
    printf("Nothing else to run.\n");
}