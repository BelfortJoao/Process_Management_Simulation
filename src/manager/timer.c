#include "timer.h"


void initializeTimer(Timer *timer)
{
    *timer = 0;
}


void timeUp(Timer *time)
{
    *time += 1;
}