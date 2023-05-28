/**
 * @file process_manager.h
 * @brief Header file containing function declarations and struct definitions for the ProcessManager module.
 */

#ifndef SRC_PROCESS_MANAGER_H
#define SRC_PROCESS_MANAGER_H

#include <stdbool.h>
#include "../cpu/core.h"
#include "art_counter.h"
#include "../processtable/process_table.h"
#include "../cpu/cpu.h"

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
 * @param filename The name of the file containing the Core and process information.
 * @param numberOfCores The number of cores in the CPU.
 * @return A pointer to the Process Manager struct if successful, NULL otherwise.
 */
ProcessManager *initializeProcessManagerFromFile(char *filename, int numberOfCores);


/**
 * @brief Blocks a process for a given amount of time.
 *
 * @param processManager Pointer to the Process Manager struct.
 * @param blockTime The amount of time to block the process for.
 * @param coreNum The core in witch the process is running.
 */
void blockProcess(ProcessManager *processManager, int blockTime, int typeOfScheduler, int coreNum);


/**
 * @brief Schedules a process to run on the Core.
 *
 * @param processManager Pointer to the process manager.
 * @param coreNum The core which the process is running.
 */
void scheduleProcess(ProcessManager *processManager, int typeOfScheduler, int coreNum);


/**
 * @brief Ends the currently executing process.
 *
 * @param processManager Pointer to the process manager.
 *
 * @param coreNum The core in which the process is running.
 */
void endProcess(ProcessManager *processManager, int typeOfScheduler, int coreNum);


/**
 * @brief Executes the next process on the ready queue.
 *
 * @param processManager Pointer to the process manager.
 * @param coreNum The core which the process is running.
 */
void execute(ProcessManager *processManager, int typeOfScheduler, int coreNum);


/**
 * @brief Continues executing the current process if the Core is not empty,
 * or executes the next process on the ready queue if it is.
 *
 * @param processManager Pointer to the process manager.
 */
void processExecuting(ProcessManager *processManager, int typeOfScheduler);


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
 * @param coreNum The core in which the process is running.
 */
void processCP(ProcessManager *processManager, int PcPlus, int coreNum);


/**
 * @brief Rewinds the current process from the given file and inserts it into the process table.
 *
 * @param processManager Pointer to the process manager struct.
 * @param filename Pointer to the name of the file to rewind the process from.
 * @param coreNum The core which the process is running.
 */
void processRewind(ProcessManager *processManager, char *filename, int coreNum);


/**
 * @brief Updates the process table with the execution information of the current process.
 *
 * @param processManager Pointer to the process manager struct.
 * @param coreNum Core which the process is running.
 */
void attExec(ProcessManager *processManager, int coreNum);

/**
 * @brief Adds 1 to the Program Counter
 *
 * @param processManager Pointer to the process manager struct.
 * @param coreNum Core which the Program Counter is being increased
 */
void upCPU(CPU *cpu, int coreNum);

/**
 * @brief Interprets the instruction of the current process and executes the appropriate action.
 *
 * @param processManager Pointer to the process manager struct.
 * @param coreNum Core which will interpret the instruction of its current process.
 */
void upperInterpreter(ProcessManager *processManager, int typeOfScheduler, int coreNum);


/**
 * @brief Frees the allocated memory for the process manager struct and its elements.
 *
 * @param processManager Pointer to the process manager struct.
 */
void freeProcessManager(ProcessManager *processManager);


#endif /* SRC_PROCESS_MANAGER_H */