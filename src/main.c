#include <stdio.h>
#include "Manager/manager.h"

int main() {
    computer comp;
    initComputer(&comp,"/home/belfort/Documentos/GitHub/Process_Management_Simulation/src/file.txt");
    for (int i = 0; i < 20; ++i) {
        processExecuting(&comp);
        printProcessTable(&comp.proctb);
    }

    return 0;
}
