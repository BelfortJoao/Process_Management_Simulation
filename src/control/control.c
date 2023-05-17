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
#define U "u"
#define M "m"
#define I "i"


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




int runControl(Control *control) {
    char file[CHAR_MAX];
    char file_command[CHAR_MAX];
    int input_type;

    printf("Type the name of the file (under '/files/'): ");
    scanf("%s", file);

    printf("Inputs by file or command line? (1-file, 2-command line): ");
    scanf("%d", &input_type);


    control->processManager = initializeProcessManagerFromFile(file);

    if (!control->processManager) {
        return ERROR;
    }

    // Cria um pipe para comunicação entre pai e filho
    int fd[2];

    if (pipe(fd) == ERROR) {
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

        while (read(fd[0], &command, 1)) {
            if (command == 'u') // Se receber um 'u' do pipe, executa o processo
            {
                processExecuting(control->processManager);
            } else if (command == 'i') // Se receber um 'i' do pipe, cria um processo e imprime "PRINT"
            {
                int newPid = fork();

                if (newPid == ERROR) // Verifica se houve erro no fork
                {
                    printf(FORK_ERROR);
                    return ERROR;
                } else if (newPid == CHILD) {
                    printProcessTable(control->processManager->processTable);
                    printState(control->processManager->processTable->ready);
                    exit(2);
                } else if (newPid == PARENT) {
                    wait(NULL);
                }
            } else if (command == 'M') // Se receber um 'm' do pipe, cria um processo e imprime "TEMPO DE RESPOSTA"
            {
                int newPid = fork();

                if (newPid == ERROR) // Verifica se houve erro no fork
                {
                    printf(FORK_ERROR);
                    return ERROR;
                } else if (newPid == CHILD) {
                    printAverageResponseTime(control->printer);
                    exit(3);
                } else // Processo pai
                {
                    wait(NULL);
                }
            }
        }
    }

    else// Processo pai
    {
        close(fd[0]); // Close pipe's read.
        char command[CHAR_MAX];


        if (input_type == 1) {

            printf("Type the file name (under '/files/'): ");
            scanf("%s", file_command);

            char filePath[strlen(FILES_FOLDER) + strlen(file_command) + 1];
            strcpy(filePath, FILES_FOLDER);
            strcat(filePath, file_command);

            FILE *inputFile = fopen(filePath, "r");

            if (!inputFile) {
                printf(FILE_ERROR);
                return ERROR;
            }

            //Pegar linha por linha e inderir no vetor command

            while (fgets(command, CHAR_MAX, inputFile)) {
                for (int i = 0; i < strlen(command); i++) {
                    switch (toupper(command[i])) {
                        case 'M':
                            write(fd[1], "m", 1); // Envia um 'm' para o pipe do filho
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
        else{
            while(true){

                if (!scanf("%s", command)){
                    cleanStdin();
                    continue;
                }

                for (int i = 0; i < strlen(command); i++) {
                    switch (toupper(command[i])) {
                        case 'M':
                        write(fd[1], "m", 1); // Envia um 'm' para o pipe do filho
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




