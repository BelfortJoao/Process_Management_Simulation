#include <stdio.h>
#include <stdlib.h>
#include "../error/error.h"

#include "timer.h"

void initializeTimer(Timer *timer)
{
    *timer = 0;
}

void timeUp(Timer *time)
{
    *time += 1;
}

void timeDown(Timer *time)
{
    *time -= 1;
}

void freeTimer(Timer *timer)
{
    if (timer)
    {
        free(timer);
    }
}