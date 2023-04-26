#include <malloc.h>
#include "timer.h"

void initTime(Timer *time) {
    time = (int *) malloc(sizeof(int));
    *time = -1;
}

void timeUp(Timer *time) {
    *time += 1;
}

void timeDown(Timer *time) {
    *time -= 1;
}