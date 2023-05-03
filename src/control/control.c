#include <stdio.h>
#include <ctype.h>
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
    char resp = ' ';
    char file[CHAR_MAX];

    printf("Type the name of the file (under '/files/'): ");
    scanf("%s", file);

    initComputer(control->processManager, file);

    printf("Digite um comando(U, I ou M)");
    while (1)
    {
        if (resp == '\n')
        {
            printf("Comando: ");
        }
        resp = getchar();
        if (resp != '\n')
        {
            if (islower(resp))
            {
                resp = toupper(resp);
            }

            switch (resp)
            {
                case 'M':
                    printAverageResponseTime(control->printer);
                    return;
                case 'U':
                    processExecuting(control->processManager);
                    continue;
                case 'I':
                    printProcessTable(&control->processManager->processTable);
                    continue;
                default:
                    printf("%c", resp);
                    printf("Erro Comando não reconhecido\n");
            }
            printf("Comando: ");
            resp = getchar();
        }
    }
}