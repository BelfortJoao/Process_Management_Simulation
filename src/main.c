#include <stdio.h>
#include "manager/manager.h"

int main() {
    Computer comp;
    initComputer(&comp,"/home/belfort/Documentos/GitHub/Process_Management_Simulation/src/file.txt");
    for (int i = 0; i < 20; ++i) {
        processExecuting(&comp);
        printProcessTable(&comp.processTable);
    }

    return 0;
}
