///home/belfort/Documentos/GitHub/Process_Management_Simulation/files/file.txt
#include <malloc.h>
#include "control/control.h"

int main()
{
    Control *control = initializeControl();
    runControl(control);
    return 0;
}