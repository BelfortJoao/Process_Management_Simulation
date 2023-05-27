#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include "../error/error.h"
#include "../printer/printer.h"

#include "control.h"
#include "../input/input.h"

#define PRINTER_DEFAULT_SIZE 10
#define ERROR (-1)
#define PARENT 1
#define CHILD 0
#define FILES_FOLDER "../files/"
#define MAX_COMMANDS 100000


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
    char fileCommand[CHAR_MAX];
    int input_type;
    int typeOfScheduler;
    int numberOfCores;

    printf("\nHow many cores will the CPU have? (): ");
    if (!scanf("%d", &numberOfCores))
    {
        cleanStdin();
        return -1;
    }

    printf("Type the name of the file (under '/files/'): ");
    if (!scanf("%s", file))
    {
        cleanStdin();
        return -1;
    }

    printf("\nWhat type of scheduling? (1 - FIFO Priority, 2 - Lottery Priority): ");
    if (!scanf("%d", &typeOfScheduler))
    {
        cleanStdin();
        return -1;
    }

    printf("\nInputs by file or command line? ( 1 - file | 2 - command line): ");
    if (!scanf("%d", &input_type))
    {
        cleanStdin();
        return -1;
    }

    control->processManager = initializeProcessManagerFromFile(file, numberOfCores);

    if (!control->processManager)
    {
        return ERROR;
    }

    char command[MAX_COMMANDS];

    // Se o input for por arquivo
    if (input_type == 1)
    {

        printf("\nType the file name (under '/files/'): ");
        scanf("%s", fileCommand);

        char filePath[strlen(FILES_FOLDER) + strlen(fileCommand) + 1];
        strcpy(filePath, FILES_FOLDER);
        strcat(filePath, fileCommand);

        FILE *inputFile = fopen(filePath, "r");

        if (!inputFile)
        {
            printf(FILE_ERROR, filePath);
            return ERROR;
        }

        char character;
        int index = 0;

        while ((character = (char) fgetc(inputFile)) != EOF)
        {
            //se for /n ou /r, ignora
            if (character == '\n' || character == '\r')
            {
                continue;
            }

            if (index >= MAX_COMMANDS - 1)
            {
                break;
            }

            switch (toupper(character))
            {
                case 'M':
                    sleep(1);
                    printf("ENCERRANDO PROCESSO");
                    printAverageResponseTime(calcAverageResponseTime(control->processManager->artCounter));
                    return 0;
                case 'U':
                    processExecuting(control->processManager, typeOfScheduler);
                    break;
                case 'I':
                    sleep(1);
                    printProcessTable(control->processManager->processTable);
                    printState(control->processManager->processTable->ready);
                    printBlocked(control->processManager->processTable->blockedQueue);
                    getchar();
                    break;
                default:
                    printf(INVALID_COMMAND, character);
                    break;
            }

            index++;
        }

        fclose(inputFile);
    }
    else // Se o input for por linha de comando
    {
        printf("\nType the commands: ");

        while (true)
        {
            if (!scanf("%s", command))
            {
                cleanStdin();
                continue;
            }

            for (int i = 0; i < strlen(command); i++)
            {
                switch (toupper(command[i]))
                {
                    case 'M':
                        sleep(1);
                        printf("ENCERRANDO PROCESSO");
                        printAverageResponseTime(calcAverageResponseTime(control->processManager->artCounter));
                        return 0;
                    case 'U':
                        processExecuting(control->processManager, typeOfScheduler);
                        break;
                    case 'I':
                        sleep(1);
                        printProcessTable(control->processManager->processTable);
                        printState(control->processManager->processTable->ready);
                        printBlocked(control->processManager->processTable->blockedQueue);
                        getchar();
                        break;
                    default:
                        printf(INVALID_COMMAND, command[i]);
                        break;
                }
            }
        }
    }

    return 0;
}