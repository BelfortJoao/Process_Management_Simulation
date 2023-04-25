#include <stdio.h>
#include "Manager/manager.h"

int main() {
    computer comp;
    initComputer(&comp,"file.txt");
    processExecuting(&comp);
    processExecuting(&comp);
    processExecuting(&comp);
    processExecuting(&comp);
    processExecuting(&comp);
    processExecuting(&comp);
    processExecuting(&comp);
    processExecuting(&comp);
    processExecuting(&comp);
    processExecuting(&comp);
    processExecuting(&comp);
    printProcessTable(&comp.proctb);
    processExecuting(&comp);
    processExecuting(&comp);

    return 0;
}
