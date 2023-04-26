//
// Created by belfort on 4/22/23.
//

#include <limits.h>
#include "process.h"
// função para inicializar a memória de um processo com valores zero
void initMemory(Process* proc, int lengthMem) {
    proc->mem= (int*) calloc(lengthMem, sizeof(int)); // aloca o vetor de memória e o preenche com zeros
    proc->lengthMem=lengthMem;
}

void declareVar(Process* proc, int position){
    proc->mem[position]=0;
}

void changeVar(Process* proc, int possition, int value){
    proc->mem[possition]= value;
}

void addVar(Process* proc, int possition, int value){
    proc->mem[possition]+= value;
}
void subVar(Process* proc, int possition, int value){
    proc->mem[possition]-= value;
}

void initProcess(Process* proc, char* name) {
    FILE* archive= fopen(name, "r");
    if (archive == NULL) {
        printf("Erro ao abrir o archive.\n");
        return;
    }
    printf("Abrindo arquivo\n");
    // conta o número de linhas no archive
    int numLines = 1;
    char c = getc(archive);
    while (c != EOF) {
        if (c == '\n') {
            numLines++;
        }
        c = getc(archive);
    }
    rewind(archive);

    // aloca o array dinamicamente
    proc->prog = (char**) malloc(numLines * sizeof(char*));
    for (int i = 0; i < numLines; i++) {
        proc->prog[i] = (char*) malloc(CHAR_MAX * sizeof(char));
    }

    // lê o archive linha por linha e preenche o array
    int linhaAtual = 0;
    while (fgets(proc->prog[linhaAtual], CHAR_MAX, archive) != NULL) {
        linhaAtual++;
    }
    proc->numLines = linhaAtual;

    fclose(archive);
}

void excludeProcess(Process* proc) {
    // desaloca o array de programa
    for (int i = 0; i < proc->numLines; i++) {
        free(proc->prog[i]);
    }

}

Process* generateNewProcess(Process* proc) {
    // aloca memória para o novo processo
     Process* newprocess = ( Process*) malloc(sizeof( Process));

    // copia o array de programa do processo atual para o novo processo
    newprocess->numLines = proc->numLines;
    newprocess->prog = (char**) malloc(newprocess->numLines * sizeof(char*));
    for (int i = 0; i < newprocess->numLines; i++) {
        newprocess->prog[i] = strdup(proc->prog[i]);
    }

    // copia o vetor de memória do processo atual para o novo processo
    newprocess->lengthMem = proc->lengthMem;
    newprocess->mem = (int*) malloc( newprocess->numLines * sizeof(int));
    for (int i = 0; i < newprocess->lengthMem; i++) {
        newprocess->mem[i] = proc->mem[i];
    }

    // retorna o novo processo criado
    return newprocess;
}
void printMem(Process* proc) {
    printf("\nMemory:\n");
    for (int i = 0; i < proc->lengthMem; i++) {
        printf("%d ", proc->mem[i]);
    }
    printf("\n");
}

void printProg(Process* proc) {
    printf("Program:\n");
    for (int i = 0; i < proc->numLines; i++) {
        printf("%s\n", proc->prog[i]);
    }
}
