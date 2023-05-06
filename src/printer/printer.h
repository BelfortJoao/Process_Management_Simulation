/**
 * @file printer.h
 * @brief Header file for Printer struct and functions to manage printing data to the console.
 */

#ifndef SRC_PRINTER_H
#define SRC_PRINTER_H

#include "../manager/processtable.h"
#include "../manager/ready.h"


/**
 * @brief Struct to store data related to printing to the console.
 *
 * This struct contains a two-dimensional array to store response times and a size variable to track the size of the array.
 */
typedef struct
{
    int **responseTimes; /**< Two-dimensional array to store response times */
    int size; /**< Size of the responseTimes array */
} Printer;


/**
 * Initializes a Printer with the given size.
 *
 * @param size the size of the Printer
 * @return a pointer to the newly allocated Printer, or NULL if allocation failed
 */
Printer *initializePrinter(int size);


/**
 * Prints a message indicating the average response time.
 *
 * @param printer a pointer to the Printer to use for printing
 */
void printAverageResponseTime(Printer *printer);


/**
 * Prints the contents of the given ProcessTable.
 *
 * @param processTable a pointer to the ProcessTable to print
 */
void printProcessTable(ProcessTable *processTable);


/**
 * Prints a message indicating that memory has been initialized.
 *
 * @param initialMemorySize the size of the initialized memory
 */
void printInitMem(int initialMemorySize);


/**
 * Prints a message indicating that a memory position has been cleared.
 *
 * @param position the position that has been cleared
 */
void printDeclareVar(int position);


/**
 * Prints a message indicating that the value at a memory position has changed.
 *
 * @param position the position whose value has changed
 * @param value the new value of the memory position
 */
void printChangeVar(int position, int value);


/**
 * Prints a message indicating that a memory position has been increased.
 *
 * @param position the position whose value has been increased
 * @param increaseValue the amount by which the memory position has been increased
 */
void printAddVar(int position, int increaseValue);


/**
 * Prints a message indicating that a memory position has been decreased.
 *
 * @param position the position whose value has been decreased
 * @param decreaseValue the amount by which the memory position has been decreased
 */
void printSubVar(int position, int decreaseValue);


/**
 * Prints a message indicating that a process has been blocked.
 *
 * @param timeUnits the number of time units for which the process has been blocked
 */
void printBlkProcess(int timeUnits);


/**
 * Prints a message indicating that a process is terminating.
 */
void printTerProcess();


/**
 * Prints a message indicating that a process is being copied.
 */
void printCopyProcess();


/**
 * Prints a message indicating that a file is being read.
 *
 * @param arg1 the name of the file being read
 */
void printReadFile(char *arg1);


/**
 * Prints the state of the given Ready queue.
 *
 * @param rs a pointer to the Ready queue to print
 */
void printState(Ready *rs);


/**
 * Prints a message indicating that a queue is full.
 */
void printFullQueue();


/**
 * Prints a message indicating that a process was not found.
 */
void printProcessNotFound();


/**
 * Prints a message indicating that a queue is empty.
 */
void printEmptyQueue();


/**
 * Prints a message indicating that there is nothing left to execute.
 */
void printFinishExe();


#endif /* SRC_PRINTER_H */