/**
 * @file process.h
 * @brief Header file for process module.
 * Contains function declarations for process creation, memory management, and information printing.
 */


#ifndef SRC_PROCESS_H
#define SRC_PROCESS_H

#define RUNNING "RUNNING"
#define BLOCKED "BLOCKED"
#define READY "READY"

typedef int *Memory;
typedef char **Program;

typedef struct
{
    Memory memory; /**< Pointer to the memory allocated for the process */
    Program program; /**< The program code as an array of strings */
    int numLines; /**< The number of lines in the program */
    int memorySize; /**< The size of the memory allocated for the process */
} Process;


/**
 * @brief Initializes a new process.
 * Allocates memory for the new process and returns a pointer to it.
 *
 * @return Process* Pointer to the newly created process.
 */
Process *initializeProcess();


/**
 * @brief Gets the number of lines in a file.
 * Takes a file pointer as input, counts the number of lines in the file,
 * then returns the number of lines.
 *
 * @param file Pointer to the file to count lines in.
 * @return int Number of lines in the file.
 */
int getNumberOfLinesInFile(FILE *file);


/**
 * @brief Initializes a new process by reading a program from a file.
 * Takes a filename as input, reads the program from the file,
 * then returns a pointer to the new process.
 *
 * @param filename Name of the file to read the program from.
 * @return Process* Pointer to the newly created process.
 */
Process *initializeProcessFromFile(char *filename);


/**
 * @brief Creates a copy of an existing process.
 * Takes a process pointer as input, creates a new process with the same program and memory,
 * then returns a pointer to the new process.
 *
 * @param processToCopy Pointer to the process to copy.
 * @return Process* Pointer to the newly created process.
 */
Process *generateProcessCopy(Process *processToCopy);


/**
 * @brief Initializes the memory for a process.
 * Takes a process pointer and a memory size as input, allocates memory for the process,
 * then initializes each memory location to 0.
 *
 * @param process Pointer to the process to initialize memory for.
 * @param memorySize Number of memory locations to allocate for the process.
 */
void initializeProcessMemory(Process *process, int memorySize);


/**
 * @brief Clears the memory at a given location for a process.
 * Takes a process pointer and a memory location as input, sets the memory at that location to 0.
 *
 * @param process Pointer to the process to clear memory for.
 * @param position Position in memory to clear.
 */
void clearProcessMemory(Process *process, int position);


/**
 * @brief Changes the value at a given memory location for a process.
 * Takes a process pointer, a memory location, and a new value as input,
 * then sets the value at that memory location to the new value.
 *
 * @param process Pointer to the process to change the memory value for.
 * @param position Position in memory to change the value of.
 * @param value New value to set at the memory location.
 */
void changeValueInProcessMemory(Process *process, int position, int value);


/**
 * @brief Increases the value at a given memory location for a process by a given value.
 * Takes a process pointer, a memory location, and a value to add as input,
 * then increases the value at that memory location by the given value.
 *
 * @param process Pointer to the process to increase the memory value for.
 * @param position Position in memory to increase the value of.
 * @param value Value to add to the memory location.
 */
void increaseValueInProcessMemory(Process *process, int position, int value);


/**
 * Reduce the value in the specified position of the process memory by the given value
 *
 * @param process   Pointer to the process whose memory is being modified
 * @param position  The position in the process memory to be modified
 * @param value     The value to subtract from the memory at the specified position
 */
void reduceValueInProcessMemory(Process *process, int position, int value);


/**
 * Print the contents of the process memory to stdout
 *
 * @param process   Pointer to the process whose memory is being printed
 */
void printProcessMemoryInfo(Process *process);


/**
 * Print the program lines of the process to stdout
 *
 * @param process   Pointer to the process whose program lines are being printed
 */
void printProcessInfo(Process *process);


/**
 * Free the memory allocated for the process and its program lines
 *
 * @param process   Pointer to the process to be freed
 */
void freeProcess(Process *process);

#endif /* SRC_PROCESS_H */