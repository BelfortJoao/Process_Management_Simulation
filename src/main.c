#include "Process.h"
#include "Manager/cpu.h"

#define FILEPATH "/home/belfort/Documentos/GitHub/Process_Management_Simulation/src/file.txt"
int main() {
    process proc;
    CPU cpu;
    initProcess(&proc, FILEPATH);
    initCPU(&cpu);
    changeProcess(&cpu,&proc,0,0,0);
    printProg(&proc);
    interpreter(&cpu);
    interpreter(&cpu);
    interpreter(&cpu);
    printMem(&proc);
    interpreter(&cpu);
    interpreter(&cpu);
    printMem(&proc);
    interpreter(&cpu);
    printMem(&proc);
    interpreter(&cpu);
    printMem(&proc);
    interpreter(&cpu);
    printMem(&proc);
    //fim
    freeCPU(&cpu);
    excludeProcess(&proc);
    return 0;

}
