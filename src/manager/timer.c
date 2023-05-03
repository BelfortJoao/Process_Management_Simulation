#include "timer.h"

void initTime(Timer *time) {
    *time = 0;
}

void timeUp(Timer *time) {
    *time += 1;
}

void timeDown(Timer *time) {
    *time -= 1;
}