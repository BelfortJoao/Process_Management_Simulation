#include "controller.h"
#include <limits.h>
void control(Control* cont){
    char resp;
    char arq[CHAR_MAX];
    printf("Arquivo de programa inicial: ");
    fgets(arq, sizeof(arq), stdin);
    initComputer(cont->comp,arq);
    while(1){
        printf("Comando: ");
        scanf("%c",&resp);
        switch (resp) {
            case 'M':
                printMedResponseTime(cont->prin);
                return;
            case 'U':
                processExecuting(cont->comp);
            case 'I':
                printProcessTable(&cont->comp->processTable);
        }
    }
}