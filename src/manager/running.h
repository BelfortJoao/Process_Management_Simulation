#ifndef SRC_RUNNING_H
#define SRC_RUNNING_H

typedef int Running; //Guarda o processo que está atualmente rodando no CPU

/**
 * @brief Initializes a new Running process.
 *
 * @return A pointer to the initialized Running process.
 * @retval NULL If there was an error allocating memory.
 */
Running *initializeRunningProcess();


/**
 * @brief Exchanges the context of the given process ID with the provided Running process.
 *
 * @param id The ID of the process to exchange the context with.
 * @param runningProcess The Running process to use for the context exchange.
 */
void contextExchange(int id, Running *runningProcess);


#endif