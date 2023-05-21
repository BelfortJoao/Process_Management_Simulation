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
    int inputType;

    printf("Type the name of the file (under '/files/'): ");
    if (!scanf("%s", file))
    {
        cleanStdin();
        return -1;
    }

    printf("Inputs by file or command line? ( 1 - file | Other - command line): ");
    if (!scanf("%d", &inputType))
    {
        cleanStdin();
        return -1;
    }

    control->processManager = initializeProcessManagerFromFile(file);

    if (!control->processManager)
    {
        return ERROR;
    }

    // Cria um pipe para comunicação entre pai e filho
    int fd[2];
    int fdFlag[2];

    if (pipe(fd) == ERROR || pipe(fdFlag) == ERROR)
    {
        printf("Error: pipe failed\n");
        return ERROR;
    }

    int processType = fork();

    if (processType == ERROR) // Verifica se houve erro no fork
    {
        printf(FORK_ERROR);
        return ERROR;
    }

    if (processType == CHILD) // Processo filho
    {
        char command;
        close(fd[1]); // Fecha o lado de escrita do pipe

        while (read(fd[0], &command, 1) > 0)
        {
            write(fdFlag[1], "R", 1);

            if (command == 'u') // Se receber um 'u' do pipe, executa o processo
            {
                processExecuting(control->processManager);
                write(fdFlag[1], "F", 1);
            }
            else if (command == 'i') // Se receber um 'i' do pipe, cria um processo e imprime "PRINT"
            {
                int newPid = fork();

                if (newPid == ERROR) // Verifica se houve erro no fork
                {
                    printf(FORK_ERROR);
                    return ERROR;
                }
                else if (newPid == CHILD)
                {
                    printProcessTable(control->processManager->processTable);
                    printState(control->processManager->processTable->ready);
                    printBlocked(control->processManager->processTable->blockedQueue);
                    write(fdFlag[1], "F", 1);
                }
                else if (newPid == PARENT)
                {
                    wait(NULL);
                }
            }
            else if (command == 'm') // Se receber um 'm' do pipe, cria um processo e imprime "TEMPO DE RESPOSTA"
            {
                int newPid = fork();

                if (newPid == ERROR) // Verifica se houve erro no fork
                {
                    printf(FORK_ERROR);
                    return ERROR;
                }
                else if (newPid == CHILD)
                {
                    printAverageResponseTime(calcAverageResponseTime(control->processManager->artCounter));
                    write(fdFlag[1], "F", 1);
                    return 0;
                }
                else // Processo pai
                {
                    wait(NULL);
                }

                close(fd[0]);
                return 0;
            }
        }

        close(fd[0]);
    }

    else// Processo pai
    {
        close(fd[0]); // Close pipe's read.

        // Se o input for por arquivo
        if (inputType == 1)
        {
            printf("Type the file name (under '/files/'): ");
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

            while ((character = (char) fgetc(inputFile)) != EOF)
            {
                //se for /n ou /r, ignora
                if (character == '\n' || character == '\r' || character == ' ')
                {
                    continue;
                }

                switch (toupper(character))
                {
                    case 'M':
                        write(fd[1], "m", 1); // Envia um 'm' para o pipe do filho
                        close(fd[1]); // Close pipe's write.
                        wait(NULL);
                        return 0;
                    case 'U':
                        write(fd[1], "u", 1); // Envia um 'u' para o pipe do filho
                        break;
                    case 'I':
                        write(fd[1], "i", 1); // Envia um 'i' para o pipe do filho
                        getchar();
                        break;
                    default:
                        printf(INVALID_COMMAND, character);
                        break;
                }

                close(fdFlag[1]);
                char comm = 'R';

                while (comm != 'F')
                {
                    read(fdFlag[0], &comm, 1);
                }

                close(fdFlag[0]);
            }

            fclose(inputFile);
        }
        else // Se o input for por linha de comando
        {
            cleanStdin();

            char command;
            while (true)
            {
                printf("\nType the commands: ");

                while ((command = (char) getchar()) != EOF && command != '\n' && command != '\r')
                {
                    switch (toupper(command))
                    {
                        case 'M':
                            write(fd[1], "m", 1); // Envia um 'm' para o pipe do filho
                            close(fd[1]); // Close pipe's write.
                            wait(NULL);
                            return 0;
                        case 'U':
                            write(fd[1], "u", 1); // Envia um 'u' para o pipe do filho
                            break;
                        case 'I':
                            write(fd[1], "i", 1); // Envia um 'i' para o pipe do filho
                            break;
                        default:
                            printf(INVALID_COMMAND, command);
                            break;
                    }

                    char comm = 'R';

                    while (comm != 'F')
                    {
                        read(fdFlag[0], &comm, 1);
                    }
                }
            }
        }
    }

    return 0;
}