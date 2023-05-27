#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
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

char *generateRandomFile(char *filename)
{
    srandom(time(NULL));
    int fileCount = 1;
    int randomCommand = random() % 5;
    int numberOfVariables = random() % 10;
    char filePath[strlen(FILES_FOLDER) + 11];
    char numString[9];
    char line[256];
    FILE *file;

    while(1)
    {
        sprintf(filename, "file%d.txt", fileCount);
        strcpy(filePath, FILES_FOLDER);
        strcat(filePath, filename);
        file = fopen(filePath, "r");

        if(file == NULL)
        {
            file = fopen(filePath, "w");

            if(file == NULL)
            {
                return NULL;
            }

            strcpy(line, "N ");
            sprintf(numString, "%d", numberOfVariables);
            strcat(line, numString);
            fputs(line, file);
            fputc('\n', file);

            for(int i = 0; i < numberOfVariables; i++)
            {
                strcpy(line, "D ");
                sprintf(numString, "%d", i);
                strcat(line, numString);
                fputs(line, file);
                fputc('\n', file);
                strcpy(line, "V ");
                sprintf(numString, "%d %d", i, random() % 1000);
                strcat(line, numString);
                fputs(line, file);
                fputc('\n', file);
            }

            do
            {
                switch (randomCommand)
                {
                    case 0:
                        strcpy(line, "A ");
                        sprintf(numString, "%d %d", random() % numberOfVariables, random() % 1000);
                        strcat(line, numString);
                        fputs(line, file);
                        fputc('\n', file);
                        break;
                    case 1:
                        strcpy(line, "S ");
                        sprintf(numString, "%d %d", random() % numberOfVariables, random() % 1000);
                        strcat(line, numString);
                        fputs(line, file);
                        fputc('\n', file);
                        break;
                    case 2:
                        strcpy(line, "B ");
                        sprintf(numString, "%d", (random() % 6) + 1);1
                        strcat(line, numString);
                        fputs(line, file);
                        fputc('\n', file);
                        break;
                    case 3:
                        strcpy(line, "T");
                        fputs(line, file);
                        break;
                    case 4:
                        strcpy(line, "F ");
                        sprintf(numString, "%d", (random() % 6) + 1);
                        strcat(line, numString);
                        fputs(line, file);
                        fputc('\n', file);
                        break;
                    default:
                        break;
                }
                randomCommand = random() % 5;
            }while (strcmp(line, "T"));
            break;
        }
        fileCount++;
        fclose(file);
    }

    fclose(file);
    return filename;
}

int runControl(Control *control)
{
    char *filename = (char*) malloc(sizeof(char) * CHAR_MAX);
    char fileCommand[CHAR_MAX];
    int input_type;
    int typeOfScheduler;
    filename = generateRandomFile(filename);

    printf("\nWhat type of scheduling? (1 - FIFO Priority, 2 - Lottery Priority): ");
    if(!scanf("%d", &typeOfScheduler))
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

    control->processManager = initializeProcessManagerFromFile(filename);

    if (!control->processManager)
    {
        return ERROR;
    }

    // Cria um pipe para comunicação entre pai e filho
    int fd[2];

    if (pipe(fd) == ERROR)
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

        while (read(fd[0], &command, 1))
        {
            if (command == 'u') // Se receber um 'u' do pipe, executa o processo
            {
                processExecuting(control->processManager, typeOfScheduler);
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
                    exit(2);
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
                    exit(3);
                }
                else // Processo pai
                {
                    wait(NULL);
                }
            }
        }
    }

    else// Processo pai
    {
        close(fd[0]); // Close pipe's read.
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
                        write(fd[1], "m", 1); // Envia um 'm' para o pipe do filho
                        sleep(1);
                        kill(processType, SIGTERM); // Mata o processo filho
                        return 0;
                    case 'U':
                        write(fd[1], "u", 1); // Envia um 'u' para o pipe do filho
                        break;
                    case 'I':
                        sleep(1);
                        write(fd[1], "i", 1); // Envia um 'i' para o pipe do filho
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
                            write(fd[1], "m", 1); // Envia um 'm' para o pipe do filho
                            sleep(1);
                            kill(processType, SIGTERM); // Mata o processo filho
                            return 0;
                        case 'U':
                            write(fd[1], "u", 1); // Envia um 'u' para o pipe do filho
                            break;
                        case 'I':
                            write(fd[1], "i", 1); // Envia um 'i' para o pipe do filho
                            break;
                        default:
                            printf(INVALID_COMMAND, command[i]);
                            break;
                    }
                }
            }
        }
    }

    return 0;
}