#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <limits.h>

#include "process.h"

#define FILES_FOLDER "../files/"
#define ERROR_FILE "Couldn't open the file at '%s'"

// função para inicializar a memória de um processo com valores zero
void initMemory(Process *process, int lengthMem)
{
    process->memory = (int *) calloc(lengthMem, sizeof(int)); // aloca o vetor de memória e o preenche com zeros
    process->memorySize = lengthMem;
}

void declareVar(Process *process, int position)
{
    process->memory[position] = 0;
}

void changeVar(Process *process, int position, int value)
{
    process->memory[position] = value;
}

void addVar(Process *process, int position, int value)
{
    process->memory[position] += value;
}

void subVar(Process *process, int position, int value)
{
    process->memory[position] -= value;
}

int getNumberOfLinesInFile(FILE *file)
{
    int numLines = 1;
    int c;

    while ((c = fgetc(file)) != EOF)
    {
        if (c == '\n')
        {
            numLines++;
        }
    }

    rewind(file);

    return numLines;
}

void initProcess(Process *process, char *filename)
{
    char filePath[strlen(FILES_FOLDER) + strlen(filename) + 1];

    strcpy(filePath, FILES_FOLDER);
    strcat(filePath, filename);

    FILE *file = fopen(filePath, "r");

    if (!file)
    {
        printf(ERROR_FILE, filePath);
        return;
    }

    printf("Abrindo arquivo\n");

    int numLines = getNumberOfLinesInFile(file);

    // Aloca o array dinamicamente
    process->program = (char **) malloc(numLines * sizeof(char *));

    for (int i = 0; i < numLines; i++)
    {
        process->program[i] = (char *) malloc(CHAR_MAX);

        if (!fscanf(file, "%[^\n]\n", process->program[i]))
        {
            printf("Couldn't read line at position %d.", i);
        }
    }

    fclose(file);
}

void excludeProcess(Process *process)
{
    for (int i = 0; i < process->numLines; i++)
    {
        free(process->program[i]);
    }
    free(process->program);
    free(process->memory);
}

Process *generateNewProcess(Process *process)
{
    // aloca memória para o novo processo
    Process *newprocess = (Process *) malloc(sizeof(Process));

    // copia o array de programa do processo atual para o novo processo
    newprocess->numLines = process->numLines;
    newprocess->program = (char **) malloc(newprocess->numLines * sizeof(char *));
    for (int i = 0; i < newprocess->numLines; i++)
    {
        newprocess->program[i] = strdup(process->program[i]);
    }

    // copia o vetor de memória do processo atual para o novo processo
    newprocess->memorySize = process->memorySize;
    newprocess->memory = (int *) malloc(newprocess->numLines * sizeof(int));
    for (int i = 0; i < newprocess->memorySize; i++)
    {
        newprocess->memory[i] = process->memory[i];
    }

    // retorna o novo processo criado
    return newprocess;
}

void printMem(Process *process)
{
    printf("\nMemory:\n");
    for (int i = 0; i < process->memorySize; i++)
    {
        printf("%d ", process->memory[i]);
    }
    printf("\n");
}

void printProg(Process *process)
{
    printf("Program:\n");
    for (int i = 0; i < process->numLines; i++)
    {
        printf("%s\n", process->program[i]);
    }
}
