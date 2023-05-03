#include <stdio.h>
#include <stdlib.h>

#include "printer.h"

Printer *initializePrinter(int size)
{
    Printer *printer = (Printer *) malloc(sizeof(Printer));

    if (printer == NULL)
    {
        return NULL;
    }

    printer->size = size;

    return printer;
}

void printAverageResponseTime(Printer *printer)
{
    printf("EU NÃO SEI O TEMPO MEDIO DE RESPOSTA POHA");
}