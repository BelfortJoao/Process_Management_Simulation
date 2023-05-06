#include "controller.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

void control(Control *cont) {
        
    int c;
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

    initComputer(cont->comp, arq);
    while (1) {
        if(resp != '\n')
            printf("Digite um comando(U, I ou M): ");
        fflush(stdout);
        resp=getchar();
        if(resp != '\n') {
            if (islower(resp))
                resp = toupper(resp);
            switch (resp) {
                case 'M':
                    printMedResponseTime(cont->printer);
                    return;
                case 'U':
                    processExecuting(cont->comp);
                case 'I':
                    printProcessTable(&cont->comp->processTable);
                    printState(cont->comp->processTable.readyArray);
                default:
                    printf("Erro: Comando não reconhecido\n");
            }
        }
        else continue;
    }
    
/*
    while (1) {
        printf("Digite um comando(U, I ou M): ");
        getchar();
        resp=getchar();
        //if (resp != '\n') {
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
                        printf("Erro: Comando não reconhecido\n");
            }
        //}
    }*/
}