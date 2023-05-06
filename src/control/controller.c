#include "controller.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

void control(Control *cont) {

    char resp;
    char arq[CHAR_MAX];

    printf("Arquivo de programa inicial: ");
    fgets(arq, sizeof(arq), stdin);
    fflush(stdin);
    size_t len = strlen(arq);
    while (len > 0 && isspace(arq[len - 1])) {
        len--;
    }
    arq[len] = '\0';
    printf("%s\n", arq);

    initComputer(cont->comp, arq);
    printf("Digite um comando(U, I ou M)");
    while (1) {
        if (resp == '\n'| resp == NULL)
            printf("Comando: ");
            resp=getchar();
        if (resp != '\n') {
            if (islower(resp))
                resp = toupper(resp);

            switch (resp) {
                case 'M':
                    printMedResponseTime(cont->printer);
                    return;
                case 'U':
                    processExecuting(cont->comp);
                    continue;
                case 'I':
                    printProcessTable(&cont->comp->processTable);
                    printState(cont->comp->processTable.readyArray);
                    continue;
                default:
                        printf("%c", resp);
                        printf("Erro Comando não reconhecido\n");
            }
            printf("Comando: ");
            resp=getchar();
        }
    }
}