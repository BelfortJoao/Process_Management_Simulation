//
// Created by belfort on 4/22/23.
//

#include <malloc.h>
#include "timer.h"
 void timeUp(timer* t){
    *t+=1;
}
void initTime(timer* t){
    *t=-1;
}