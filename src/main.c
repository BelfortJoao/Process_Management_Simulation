#include <stdio.h>
#include "Manager/manager.h"

int main() {
    computer comp;
    initComputer(&comp,"/home/belfort/Documentos/GitHub/Process_Management_Simulation/src/file.txt");
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
    return 0;
}
