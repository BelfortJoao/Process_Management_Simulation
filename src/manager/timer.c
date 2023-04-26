#include "timer.h"

void initTime(Timer* timer) {
    *timer = 0;
}

void timeUp(Timer* timer) {
    *timer += 1;
}

void timeDown(Timer* timer) {
    *timer -= 1;
}