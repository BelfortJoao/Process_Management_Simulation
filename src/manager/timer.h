#ifndef SRC_TIMER_H
#define SRC_TIMER_H


typedef int Timer; //estrutura que mede o tempo


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


/**
 * @brief Decreases the given Timer by 1.
 *
 * @param time A pointer to the Timer to decrement.
 */
void timeDown(Timer *time);


#endif