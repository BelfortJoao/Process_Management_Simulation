/**
 * @file art_counter.h
 * @brief Header file containing function declarations and struct definitions for the average response time counter module.
 */

#ifndef SRC_ART_COUNTER_H
#define SRC_ART_COUNTER_H

/**
 * @brief Struct containing the necessary data for the average response time counter.
 */
typedef struct ArtCounter
{
    int totalTime;
    int numberOfFinishedProcesses;
}ArtCounter;

/**
 * @brief Initializes the average response time counter.
 *
 * @return A pointer to the average response time counter struct if successful, NULL otherwise.
 */
ArtCounter *initializeArtCounter();

/**
 * @brief Adds the time when the process was finished to the total time and 1(one) to the number of finished processes.
 *
 * @param artCounter Pointer to the average return time counter.
 * @param processInitTime CPU time when the process that is beeing finished started.
 * @param currTime Current time of the CPU.
 */
void addFinishedProcess (ArtCounter *artCounter, int processInitTime, int currTime);

/**
 * @brief Calculates the average response time.
 *
 * @param artCounter Pointer to the average return time counter.
 */
double calcAverageResponseTime(ArtCounter *artCounter);

/**
 * @brief Frees the memory used by the average response time counter.
 * @param artCounter Pointer to the average response time counter to free
 */
void freeArtCounter(ArtCounter *artCounter);

#endif /* SRC_ART_COUNTER_H */