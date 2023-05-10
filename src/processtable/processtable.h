/**
 * @file ProcessTable.h
 * @brief This file contains the ProcessTable data structure and its functions.
 */

#ifndef SRC_PROCESSTABLE_H
#define SRC_PROCESSTABLE_H

#include <stdbool.h>
#include "../process/process.h"
#include "../manager/timer.h"
#include "../manager/ready.h"
#include "../manager/blocked.h"
#include "../manager/running.h"
#include "processtablecellqueue.h"


/**
 * @struct ProcessTable
 * @brief This struct represents a table of processes.
 */
typedef struct ProcessTable
{
    ProcessTableCellQueue *processTableCellQueue;
    int size;
    Ready *readyArray; /**< An array of ready processes. */
    Blocked *blockedArray; /**< An array of blocked processes. */
    Running running; /**< The process currently being executed. */
} ProcessTable;


/**
 * Initializes a new ProcessTable with the specified initial capacity.
 *
 * @param initialCapacity the initial capacity of the table
 * @return a pointer to the new ProcessTable, or NULL if initialization failed
 */
ProcessTable *initializeProcessTable(int initialCapacity);


/**
 * Adds a new process to the ProcessTable.
 *
 * @param processTable the ProcessTable to add the process to
 * @param filename the name of the file containing the process code
 * @param parentProcessId the ID of the parent process, or -1 if the new process is a root process
 * @param clock the current value of the system clock
 * @return true if the process was successfully added, false otherwise
 */
bool addProcessTableProcess(ProcessTable *processTable, char *filename, int parentProcessId, int clock);


/**
 * @brief Deletes a process from the process table.
 *
 * @param id The ID of the process to be deleted.
 * @param processTable The process table from which the process will be deleted.
 */
void deleteProcessTableProcess(int id, ProcessTable *processTable);


/**
 * @brief Frees the memory allocated for a process table and its contents.
 *
 * @param processTable The process table to be freed.
 */
void freeProcessTable(ProcessTable *processTable);


/**
 * @brief Copies a process into the process table.
 *
 * @param processTable The process table to copy the process into.
 * @param timer The initial timer value for the process.
 * @param PcPlus The amount to add to the program counter of the new process.
 *
 * @return True if the process was successfully copied, false otherwise.
 */
bool copyProcess(ProcessTable *processTable, Timer timer, int PcPlus);


#endif /* SRC_PROCESSTABLE_H */