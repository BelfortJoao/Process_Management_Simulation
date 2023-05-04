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