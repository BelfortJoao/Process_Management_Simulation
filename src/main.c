#include <stdio.h>
#include "Manager/manager.h"

int main() {
    computer comp;
    initComputer(&comp,"/file.txt");
    for (int i = 0; i < 15; ++i) {
        processExecuting(&comp);
        printProcessTable(&comp.proctb);
    }

    return 0;
}
