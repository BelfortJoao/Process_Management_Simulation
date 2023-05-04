#ifndef SRC_TIMER_H
#define SRC_TIMER_H

typedef int Timer; //estrutura que mede o tempo

Timer *initializeTimer();

void timeUp(Timer *time);

void timeDown(Timer *time);

#endif