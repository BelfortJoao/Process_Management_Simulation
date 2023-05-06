#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <limits.h>
#include "../error/error.h"
#include "../printer/printer.h"

#include "control.h"

#define PRINTER_DEFAULT_SIZE 10


/**
 * @brief Allocates memory for a new Control struct and initializes its process manager and printer.
 *
 * @return Pointer to the newly created Control struct, or NULL if memory allocation fails.
 */
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

    bool printFlag = false;

    while (true)
    {
        char command = (char) getchar();

        if (!printFlag)
        {
            printf("\nType a command (U, I or M): ");
            printFlag = true;
        }

        if (command == '\n')
        {
            continue;
        }

        switch (toupper(command))
        {
            case 'M':
                printAverageResponseTime(control->printer);
                return 0;
            case 'U':
                processExecuting(control->processManager);
                break;
            case 'I':
                printProcessTable(control->processManager->processTable);
                printState(control->processManager->processTable->readyArray);
                break;
            default:
                printf(INVALID_COMMAND, command);
        }

        printf("\nType a command (U, I or M): ");
        fflush(stdout);
    }
}