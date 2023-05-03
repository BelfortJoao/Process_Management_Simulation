#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <limits.h>

#include "control.h"

#define PRINTER_DEFAULT_SIZE 10

Control *initializeControl()
{
    Control *control = (Control *) malloc(sizeof(Control));

    if (!control)
    {
        return NULL;
    }

    control->processManager = initializeProcessManager();
    control->printer = initializePrinter(PRINTER_DEFAULT_SIZE);

    return control;
}

void runControl(Control *control)
{
    char file[CHAR_MAX];

    printf("Type the name of the file (under '/files/'): ");
    scanf("%s", file);

    initComputer(control->processManager, file);

    printf("Comando: ");
    processExecuting(control->processManager);
}