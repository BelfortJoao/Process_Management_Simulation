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

    control->processManager = initializeProcessManager();

    if (!control->processManager)
    {
        return NULL;
    }

    control->printer = initializePrinter(PRINTER_DEFAULT_SIZE);

    if (!control->printer)
    {
        return NULL;
    }

    return control;
}

int runControl(Control *control)
{
    char file[CHAR_MAX];

    printf("Type the name of the file (under '/files/'): ");
    scanf("%s", file);

    control->processManager = initializeProcessManagerFromFile(file);

    if (!control->processManager)
    {
        return -1;
    }

    while (true)
    {
        char command = (char) getchar();

        if (command != '\n')
        {
            switch (toupper(command))
            {
                case 'M':
                    printAverageResponseTime(control->printer);
                    return 0;
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

        printf("Type a command (U, I or M): ");
        fflush(stdout);
    }
}