//
// Created by belfort on 4/22/23.
//

#include <limits.h>
#include "Process.h"
void initMemory(struct process* proc) {
    char* firstLine = proc->prog[0];
    int lengthMem;
    char lixo;

    sscanf(firstLine, "%c %d", &lixo, &lengthMem); // extrai o valor de X da primeira linha do programa

    proc->mem = (int*) malloc(lengthMem * sizeof(int));

    // inicializa todos os elementos do vetor com zero
    for (int i = 0; i < lengthMem; i++) {
        proc->mem[i] = 0;
    }
    proc->lengthMem=lengthMem;
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