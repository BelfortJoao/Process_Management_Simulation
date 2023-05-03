#ifndef SRC_CONTROL_H
#define SRC_CONTROL_H

#include "../manager/processmanager.h"
#include "../printer/printer.h"

typedef struct
{
    ProcessManager *processManager;
    Printer *printer;
} Control;

Control *initializeControl();

void runControl(Control *control);

#endif