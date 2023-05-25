/**
 * @file process_manager.h
 * @brief Header file containing function declarations and struct definitions for the ProcessManager module.
 */

#ifndef SRC_PROCESS_MANAGER_H
#define SRC_PROCESS_MANAGER_H

#include <stdbool.h>
#include "cpu.h"
#include "art_counter.h"
#include "../processtable/process_table.h"

#define DEFAULT_INITIAL_CAPACITY 1000


/**
 * @brief Struct containing the necessary data for a Process Manager.
 */
typedef struct ProcessManager
{
    CPU *cpu; /**< Pointer to the CPU struct. */
    Timer timer; /**< Struct containing the current time. */
    ArtCounter *artCounter; /**< Pointer to the average return time struct. */
    bool kill; /**< Flag indicating if the manager should stop execution. */
    ProcessTable *processTable; /**< Pointer to the Process Table struct. */
} ProcessManager;


/**
 * @brief Allocates memory for a Process Manager struct and initializes its members.
 *
 * @return A pointer to the Process Manager struct if successful, NULL otherwise.
 */
ProcessManager *initializeProcessManager();


/**
 * @brief Initializes a Process Manager with the given filename.
 *
 * @param filename The name of the file containing the CPU and process information.
 * @return A pointer to the Process Manager struct if successful, NULL otherwise.
 */
ProcessManager *initializeProcessManagerFromFile(char *filename);


/**
 * @brief Blocks a process for a given amount of time.
 *
 * @param processManager Pointer to the Process Manager struct.
 * @param blockTime The amount of time to block the process for.
 */
void blockProcess(ProcessManager *processManager, int blockTime);

char** ReadArchive(char *filename);

/**
 * @brief Schedules a process to run on the CPU.
 *
 * @param processManager Pointer to the process manager.
 */
void scheduleProcess(ProcessManager *processManager);


/**
 * @brief Ends the currently executing process.
 *
 * @param processManager Pointer to the process manager.
 */
void endProcess(ProcessManager *processManager);


/**
 * @brief Executes the next process on the ready queue.
 *
 * @param processManager Pointer to the process manager.
 */
void execute(ProcessManager *processManager);


/**
 * @brief Continues executing the current process if the CPU is not empty,
 * or executes the next process on the ready queue if it is.
 *
 * @param processManager Pointer to the process manager.
 */
void processExecuting(ProcessManager *processManager);


/**
 * @brief Removes blocked processes that are ready to run and inserts them into the ready queue.
 *
 * @param processManager Pointer to the process manager struct.
 */
void processUnblock(ProcessManager *processManager);


/**
 * @brief Advances the clock and blocks processes that exceeded their time quantum.
 *
 * @param processManager Pointer to the process manager struct.
 */
void clockUpPC(ProcessManager *processManager);


/**
 * @brief Creates a copy of the current process and inserts it into the process table.
 *
 * @param processManager Pointer to the process manager struct.
 * @param PcPlus Integer value of program counter.
 */
void processCP(ProcessManager *processManager, int PcPlus);


/**
 * @brief Rewinds the current process from the given file and inserts it into the process table.
 *
 * @param processManager Pointer to the process manager struct.
 * @param filename Pointer to the name of the file to rewind the process from.
 */
void processRewind(ProcessManager *processManager, char *filename);


/**
 * @brief Updates the process table with the execution information of the current process.
 *
 * @param processManager Pointer to the process manager struct.
 */
void attExec(ProcessManager *processManager);


/**
 * @brief Interprets the instruction of the current process and executes the appropriate action.
 *
 * @param processManager Pointer to the process manager struct.
 */
void upperInterpreter(ProcessManager *processManager);


/**
 * @brief Frees the allocated memory for the process manager struct and its elements.
 *
 * @param processManager Pointer to the process manager struct.
 */
void freeProcessManager(ProcessManager *processManager);


#endif /* SRC_PROCESS_MANAGER_H */