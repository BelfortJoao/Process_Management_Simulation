/**
 * @file runningId.h
 * @brief Header file for the Running process functionality.
 */

#ifndef SRC_RUNNING_H
#define SRC_RUNNING_H


/**
 * @brief An integer representing a Running process.
 */
typedef int Running; /**< Stores ID of the runningId process. */

/**
 * @brief Initializes an array of running processes.
 *
 * @param numberOfRunning The maximum number of processes that can be running at the same time.
 */
Running *initializeRunning(int numberOfRunning);

/**
 * @brief Exchanges the context of the given process ID with the provided Running process.
 *
 * @param id The ID of the process to exchange the context with.
 * @param runningProcess The Running process to use for the context exchange.
 */
void contextExchange(int id, Running *runningProcess);


#endif /* SRC_RUNNING_H */