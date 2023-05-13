#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <limits.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include "../error/error.h"
#include "../printer/printer.h"

#include "control.h"
#include "../input/input.h"

#define PRINTER_DEFAULT_SIZE 10


Control *initializeControl() {
    Control *control = (Control *) malloc(sizeof(Control));

    if (!control) {
        printf(ALLOCATION_ERROR, "control");
        return NULL;
    }

    control->processManager = initializeProcessManager();

    if (!control->processManager) {
        return NULL;
    }

    control->printer = initializePrinter(PRINTER_DEFAULT_SIZE);

    if (!control->printer) {
        return NULL;
    }

    return control;
}


int runControl(Control *control) {
    char file[CHAR_MAX];

    printf("Type the name of the file (under '/files/'): ");
    scanf("%s", file);

    control->processManager = initializeProcessManagerFromFile(file);

    if (!control->processManager) {
        return -1;
    }

// Cria um pipe para comunicação entre pai e filho
    int fd[2];
    if (pipe(fd) == -1) {
        printf("Error: pipe failed\n");
        return -1;
    }

    bool printFlag = false;
    int pid = fork();

    if (pid == -1) // Verifica se houve erro no fork
    {
        printf("Error: fork failed\n");
        return -1;
    } else if (pid == 0) // Processo filho
    {
        char buf;
        close(fd[1]); // Fecha o lado de escrita do pipe

        while (read(fd[0], &buf, 1)) {
            if (buf == 'u') // Se receber um 'u' do pipe, executa o processo
            {
                processExecuting(control->processManager);
            } else if (buf == 'i') // Se receber um 'i' do pipe, cria um processo e imprime "PRINT"
            {
                int new_pid = fork();

                if (new_pid == -1) // Verifica se houve erro no fork
                {
                    printf("Error: fork failed\n");
                    return -1;
                } else if (new_pid == 0) // Processo filho
                {
                    printProcessTable(control->processManager->processTable);
                    printState(control->processManager->processTable->ready);
                    exit(2);
                } else // Processo pai
                {
                    wait(NULL);
                }
            } else if (buf == 'm') // Se receber um 'm' do pipe, cria um processo e imprime "TEMPO DE RESPOSTA"
            {
                int new_pid = fork();

                if (new_pid == -1) // Verifica se houve erro no fork
                {
                    printf("Error: fork failed\n");
                    return -1;
                } else if (new_pid == 0) // Processo filho
                {
                    printAverageResponseTime(control->printer);
                    printf("TEMPO DE RESPOSTA\n");
                    exit(3);
                } else // Processo pai
                {
                    wait(NULL);
                }
            }

        }
    } else // Processo pai
    {
        close(fd[0]); // Fecha o lado de leitura do pipe

        int j = 0;

        while (true) {
            char command = (char) getchar();

            if (!printFlag) {

                printf("\nType a command (U, I or M): ");
                printFlag = true;
            }

            if (command == '\n') {
                continue;
            }

            switch (toupper(command)) {
                case 'M':
                    write(fd[1], "m", 1); // Envia um 'm' para o pipe do filho
                    kill(pid, SIGTERM); // Mata o processo filho
                    break;
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
            fflush(stdout);
            printf("\nType a command (U, I or M): ");
        }
    }
}





