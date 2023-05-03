#ifndef SRC_PRINTER_H
#define SRC_PRINTER_H

typedef struct
{
    int *responseTimes;
    int size;
} Printer;

Printer *initializePrinter(int size);

void printAverageResponseTime(Printer *printer);

#endif