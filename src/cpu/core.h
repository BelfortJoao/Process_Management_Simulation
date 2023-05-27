/**
 * @file cpu.h
 * @brief Header file for the Core module
 */

#ifndef SRC_CORE_H
#define SRC_CORE_H

#include <stdbool.h>

#include "../process/process.h"
#include "../manager/timer.h"


enum CoreState
{
    WORKING, IDLE
};


/**
 * @struct Core
 * @brief Struct for the Core module
 */
typedef struct Core
{
    Process *runningProcess; /**< Pointer to the process that is currently runningId */
    int programCounter; /**< Program counter */
    Timer executing_timer; /**< Timer for the Core's executing time */
    Timer program_timer; /**< Timer for the current program's time */
    enum CoreState coreState;
} Core;


Core *initializeCore();


/**
 * @brief Initializes a new Core with the program stored in the specified file
 * @param filename Name of the file containing the program to load
 * @return Pointer to the new Core, or NULL if there was an error
 */
void initializeCoreFromFile(Core *core, char *filename);


/**
 * @brief Converts a string to an integer
 * @param string The string to convert
 * @return The integer representation of the string, or 0 if the conversion failed
 */
int convertStringToInt(char *string);


/**
 * @brief Interprets a single line of the program
 * @param core Pointer to the Core that is executing the program
 * @param blk Pointer to an integer that will be set to the ID of the block that the program should jump to, if applicable
 * @param file Pointer to a string that will be set to the name of the file that the program should read from, if applicable
 * @param PCPlus Pointer to an integer that will be set to the number of lines to skip after executing a "F" instruction, if applicable
 * @return An integer indicating the next action to take, as follows:
 *     - 0: Continue executing the program normally
 *     - 1: Jump to a different block of the program
 *     - 2: Terminate the program
 *     - 3: Skip a specified number of lines in the program
 *     - 4: Read input from a file
 */
int interpreter(Core *core, int *blk, char **file, int *PCPlus);


/**
 * @brief Replaces the currently runningId process with a new process
 * @param core Pointer to the Core that is executing the program
 * @param process Pointer to the new process
 * @param programCounter New value for the program counter
 * @param program_timer New value for the program timer
 * @param executing_timer New value for the executing timer
 * @return Weather if it found a free core to run the process
 */
bool changeProcess(Core *core, Process *process, int programCounter, Timer program_timer, Timer executing_timer);


/**
 * @brief Frees the memory used by a Core
 * @param core Pointer to the Core to free
 */
void freeCore(Core *core);


#endif /* SRC_CORE_H */