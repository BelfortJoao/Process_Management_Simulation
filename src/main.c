///home/belfort/Documentos/GitHub/Process_Management_Simulation/files/file.txt
#include <malloc.h>
#include "control/controller.h"

int main() {
    Control *cont = malloc(sizeof(Control));
    cont->comp = malloc(sizeof(Computer));
    cont->printer = malloc(sizeof(Print));
    control(cont);
    return 0;
}