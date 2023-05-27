/**
 * @file timer.h
 * @brief Contains functions and data structures related to time management.
 */

#ifndef SRC_TIMER_H
#define SRC_TIMER_H


typedef int Timer; /**< Struct that measures the time. */


/**
 * @brief Initializes the given Timer to 0.
 *
 * @param timer A pointer to the Timer to initialize.
 */
void initializeTimer(Timer *timer);


/**
 * @brief Increases the given Timer by 1.
 *
 * @param time A pointer to the Timer to increment.
 */
void timeUp(Timer *time);


#endif /* SRC_TIMER_H */