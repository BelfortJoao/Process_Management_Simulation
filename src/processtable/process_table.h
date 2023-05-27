/**
 * @file ProcessTable.h
 * @brief This file contains the ProcessTable data structure and its functions.
 */

#ifndef SRC_PROCESS_TABLE_H
#define SRC_PROCESS_TABLE_H

#include <stdbool.h>
#include "../process/process.h"
#include "../manager/timer.h"
#include "../ready/ready.h"
#include "../blocked/blocked_queue.h"
#include "../running/running.h"
#include "process_table_cell_queue.h"


/**
 * @struct ProcessTable
 * @brief This struct represents a table of processes.
 */
typedef struct ProcessTable
{
    ProcessTableCellQueue *processTableCellQueue;
    int nextFreeId;
    Ready *ready; /**< An array of ready processes. */
    BlockedQueue *blockedQueue; /**< An array of blocked processes. */
    Running *runningId; /**< An array of the processes currently being executed. */
} ProcessTable;


/**
 * Initializes a new ProcessTable with the specified initial capacity.
 *
 * @param initialCapacity the initial capacity of the table
 * @return a pointer to the new ProcessTable, or NULL if initialization failed
 */
ProcessTable *initializeProcessTable(int initialCapacity, int numberOfRunning);


/**
 * Adds a new process to the ProcessTable.
 *
 * @param processTable the ProcessTable to add the process to
 * @param filename the name of the file containing the process code
 * @param parentProcessId the ID of the parent process, or -1 if the new process is a root process
 * @param clock the current value of the system clock
 * @return true if the process was successfully added, false otherwise
 */
bool addProcessTableProcess(ProcessTable *processTable, Process *process, int parentProcessId, int clock);


/**
 * @brief Deletes a process from the process table.
 *
 * @param id The ID of the process to be deleted.
 * @param processTable The process table from which the process will be deleted.
 */
void deleteProcessTableProcess(int id, ProcessTable *processTable);


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


/**
 * @brief Frees the memory allocated for a process table and its contents.
 *
 * @param processTable The process table to be freed.
 */
void freeProcessTable(ProcessTable *processTable);


#endif /* SRC_PROCESS_TABLE_H */