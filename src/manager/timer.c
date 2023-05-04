#include <stdio.h>
#include <stdlib.h>
#include "../error/error.h"

#include "timer.h"

Timer *initializeTimer()
{
    Timer *timer = (Timer *) malloc(sizeof(Timer));

    if (!timer)
    {
        printf(ALLOCATION_ERROR, "process table");
        return NULL;
    }

    *timer = 0;

    return timer;
}

void timeUp(Timer *time)
{
    *time += 1;
}

void timeDown(Timer *time)
{
    *time -= 1;
}