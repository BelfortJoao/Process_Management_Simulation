#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <limits.h>
#include "../input/input.h"
#include "../error/error.h"

#include "control.h"

#define PRINTER_DEFAULT_SIZE 10

Control *initializeControl()
{
    Control *control = (Control *) malloc(sizeof(Control));

    if (!control)
    {
        printf(ALLOCATION_ERROR, "control");
        return NULL;
    }

    // I think this is useless.
//    control->processManager = initializeProcessManager();
//
//    if (!control->processManager)
//    {
//        return NULL;
//    }

    control->printer = initializePrinter(PRINTER_DEFAULT_SIZE);

    if (!control->printer)
    {
        return NULL;
    }

    return control;
}

void runControl(Control *control)
{
    char file[CHAR_MAX];

    printf("Type the name of the file (under '/files/'): ");
    scanf("%s", file);

    control->processManager = initializeProcessManagerFromFile(file);

    if (!control->processManager)
    {
        return;
    }

    while (true)
    {
        char command = ' ';

        printf("Type a command (U, I or M): ");
        cleanStdin();

        if (!scanf("%c", &command))
        {
            printf("INVALID INPUT\n");
            cleanStdin();
            continue;
        }

        switch (toupper(command))
        {
            case 'M':
                printAverageResponseTime(control->printer);
                return;
            case 'U':
                processExecuting(control->processManager);
                continue;
            case 'I':
                printProcessTable(control->processManager->processTable);
                continue;
            default:
                printf(INVALID_COMMAND, command);
        }
    }
}