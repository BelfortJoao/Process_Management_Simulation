//
// Created by belfort on 4/22/23.
//

#include <limits.h>
#include "Process.h"
// função para inicializar a memória de um processo com valores zero
void initMemory(struct process* proc, int lengthMem) {
    proc->mem= (int*) calloc(lengthMem, sizeof(int)); // aloca o vetor de memória e o preenche com zeros
    proc->lengthMem=lengthMem;
}

void declareVar(struct process* proc, int position){
    proc->mem[position]=0;
}

void changeVar(struct process* proc, int possition, int value){
    proc->mem[possition]= value;
}

void addVar(struct process* proc, int possition, int value){
    proc->mem[possition]+= value;
}
void subVar(struct process* proc, int possition, int value){
    proc->mem[possition]-= value;
}

void initProcess(struct process* proc, char* name) {
    FILE* archive= fopen(name, "r");
    if (archive == NULL) {
        printf("Erro ao abrir o archive.\n");
        return;
    }
    // conta o número de linhas no archive
    int numLines = 0;
    char c = getc(archive);
    while (c != EOF) {
        if (c == '\n') {
            numLines++;
        }
        c = getc(archive);
    }
    rewind(archive);
    proc->numLines=numLines;
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

    fclose(archive);
}

void excludeProcess(struct process* proc) {
    // desaloca o array de programa
    for (int i = 0; i < proc->lengthMem; i++) {
        free(proc->prog[i]);
    }
    free(proc->prog);

    // desaloca a memoria
    free(proc->mem);

    // desaloca o proc
    free(proc);
}

process* criarNovoProcesso(struct process* proc) {
    // aloca memória para o novo processo
    struct process* newprocess = (struct Processo*) malloc(sizeof(struct process));

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