#ifndef SRC_PRINTER_H
#define SRC_PRINTER_H

typedef struct {
    int **responseTimes;
    int size;
} Print;

void printMedResponseTime(Print *print);

#endif