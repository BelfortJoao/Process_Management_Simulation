#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <limits.h>

#include "process.h"

// função para inicializar a memória de um processo com valores zero
void initMemory(Process *process, int lengthMem) {
    process->memory = (int *) calloc(lengthMem, sizeof(int)); // aloca o vetor de memória e o preenche com zeros
    process->memorySize = lengthMem;
}

void declareVar(Process *process, int position) {
    process->memory[position] = 0;
}

void changeVar(Process *process, int position, int value) {
    process->memory[position] = value;
}

void addVar(Process *process, int position, int value) {
    process->memory[position] += value;
}

void subVar(Process *process, int position, int value) {
    process->memory[position] -= value;
}

void initProcess(Process *process, char *name) {
    FILE *archive = fopen(name, "r");
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
    process->program = (char **) malloc(numLines * sizeof(char *));
    for (int i = 0; i < numLines; i++) {
        process->program[i] = (char *) malloc(CHAR_MAX * sizeof(char));
    }

    // lê o archive linha por linha e preenche o array
    int linhaAtual = 0;
    while (fgets(process->program[linhaAtual], CHAR_MAX, archive) != NULL) {
        linhaAtual++;
    }
    process->numLines = linhaAtual;

    fclose(archive);
}

void excludeProcess(Process *process) {
    // desaloca o array de programa
    for (int i = 0; i < process->numLines; i++) {
        free(process->program[i]);
    }

}

Process *generateNewProcess(Process *process) {
    // aloca memória para o novo processo
    Process *newprocess = (Process *) malloc(sizeof(Process));

    // copia o array de programa do processo atual para o novo processo
    newprocess->numLines = process->numLines;
    newprocess->program = (char **) malloc(newprocess->numLines * sizeof(char *));
    for (int i = 0; i < newprocess->numLines; i++) {
        newprocess->program[i] = strdup(process->program[i]);
    }

    // copia o vetor de memória do processo atual para o novo processo
    newprocess->memorySize = process->memorySize;
    newprocess->memory = (int *) malloc(newprocess->numLines * sizeof(int));
    for (int i = 0; i < newprocess->memorySize; i++) {
        newprocess->memory[i] = process->memory[i];
    }

    // retorna o novo processo criado
    return newprocess;
}

void printMem(Process *process) {
    printf("\nMemory:\n");
    for (int i = 0; i < process->memorySize; i++) {
        printf("%d ", process->memory[i]);
    }
    printf("\n");
}

void printProg(Process *process) {
    printf("Program:\n");
    for (int i = 0; i < process->numLines; i++) {
        printf("%s\n", process->program[i]);
    }
}
