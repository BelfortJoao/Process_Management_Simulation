//
// Created by belfort on 4/22/23.
//

#ifndef SRC_PRINTER_H
#define SRC_PRINTER_H
typedef struct printer{
    int** responseTimes;
    int size;
}Print;

void printMedResponseTime(Print* print);
#endif //SRC_PRINTER_H
