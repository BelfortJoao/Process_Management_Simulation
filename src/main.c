#include <stdio.h>
#include "Manager/manager.h"

int main() {
    computer comp;
    printf("E\n");
    initComputer(&comp);
    printf("E\n");
    addProcess(&comp.proctb,"file.txt",-1,0);
    printf("E\n");

    return 0;
}
