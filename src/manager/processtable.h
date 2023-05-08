/**
 * @file ProcessTable.h
 * @brief This file contains the ProcessTable data structure and its functions.
 */

#ifndef SRC_PROCESSTABLE_H
#define SRC_PROCESSTABLE_H

#include <stdbool.h>
#include "../process/process.h"
#include "timer.h"
#include "ready.h"
#include "blocked.h"
#include "running.h"


/**
 * @struct ProcessTable
 * @brief This struct represents a table of processes.
 */
typedef struct ProcessTable
{
    int nextFreeId; /**< The next free index in the idArray. */
    int tableSize; /**< The number of processes currently in the table. */
    int tableCapacity; /**< The maximum capacity of the table. */
    int *emptyArray; /**< A flag array indicating whether each index is empty or not. */
    int *idArray; /**< An array containing the process IDs corresponding to each position. */
    Process **processArray; /**< An array of pointers to the processes. */
    int *programCounterArray; /**< An array of program counters for each process. */
    int *parentProcessArray; /**< An array containing the parent process ID for each process. */
    int *priorityIdArray; /**< An array of priority levels for each process (values between 0 and 3). */
    char **processStateArray; /**< An array of process states (strings with values "Executing", "Ready", "Blocked"). */
    Timer *initialTimerArray; /**< An array of start times for each process. */
    Timer *CPUTimerArray; /**< An array of CPU usage times for each process. */
    Ready *readyArray; /**< An array of ready processes. */
    Blocked *blockedArray; /**< An array of blocked processes. */
    Running *executingArray; /**< The process currently being executed. */
} ProcessTable;


/**
 * Initializes a new ProcessTable with the specified initial capacity.
 *
 * @param initialCapacity the initial capacity of the table
 * @return a pointer to the new ProcessTable, or NULL if initialization failed
 */
ProcessTable *initializeProcessTable(int initialCapacity);


/**
 * Searches the ProcessTable for an empty space.
 *
 * @param processTable the ProcessTable to search
 * @return the index of the first empty space, or -1 if the table is full
 */
int getProcessTableEmptySpace(ProcessTable *processTable);


/**
 * Adds a new process to the ProcessTable.
 *
 * @param processTable the ProcessTable to add the process to
 * @param filename the name of the file containing the process code
 * @param parentProcess the ID of the parent process, or -1 if the new process is a root process
 * @param clock the current value of the system clock
 * @return true if the process was successfully added, false otherwise
 */
bool addProcessTableProcess(ProcessTable *processTable, char *filename, int parentProcess, int clock);


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
void deleteProcessTable(ProcessTable *processTable);


/**
 * @brief Searches the process table for a process with a given ID.
 *
 * @param id The ID of the process to be searched for.
 * @param processTable The process table to search in.
 *
 * @return The index of the process in the process table, or -1 if the process is not found.
 */
int searchByIdInProcessTable(int id, ProcessTable *processTable);


/**
 * @brief Returns the next free ID for a new process in the process table.
 *
 * @param processTable The process table to get the next ID for.
 *
 * @return The next free ID for a new process.
 */
int nextID(ProcessTable *processTable);


/**
 * @brief Copies a process into the process table.
 *
 * @param processTable The process table to copy the process into.
 * @param proc The process to be copied.
 * @param timer The initial timer value for the process.
 * @param PcPlus The amount to add to the program counter of the new process.
 *
 * @return True if the process was successfully copied, false otherwise.
 */
bool copyProcess(ProcessTable *processTable, Process *proc, Timer timer, int PcPlus);


#endif /* SRC_PROCESSTABLE_H */