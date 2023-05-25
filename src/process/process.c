#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>
#include "../error/error.h"

#include "process.h"

#define FILES_FOLDER "../files/"


Process *initializeProcess()
{
    Process *process = (Process *) malloc(sizeof(Process));

    if (!process)
    {
        printf(ALLOCATION_ERROR, "process");
        return NULL;
    }

    process->memory = NULL;

    return process;
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


Process *initializeProcessFromFile(char *filename)
{
    Process *process = initializeProcess();

    if (process == NULL)
    {
        return NULL;
    }

    char filePath[strlen(FILES_FOLDER) + strlen(filename) + 1];

    strcpy(filePath, FILES_FOLDER);
    strcat(filePath, filename);

    FILE *file = fopen(filePath, "r");

    if (!file)
    {
        printf(FILE_ERROR, filePath);
        return NULL;
    }

    process->numLines = getNumberOfLinesInFile(file);
    process->program = (char **) malloc(process->numLines * sizeof(char *));

    if (!process->program)
    {
        printf(ALLOCATION_ERROR, "process program");
        return NULL;
    }

    for (int i = 0; i < process->numLines; i++)
    {
        process->program[i] = (char *) malloc(CHAR_MAX);

        if (!process->program[i])
        {
            printf(ALLOCATION_ERROR, "process program line");
            return NULL;
        }

        if (!fscanf(file, "%[^\r\n]\n", process->program[i]))
        {
            printf("\nCouldn't read line at position %d.\n", i);
        }
    }

    fclose(file);

    return process;
}


Process *generateProcessCopy(Process *processToCopy)
{
    Process *newProcess = initializeProcess();

    newProcess->numLines = processToCopy->numLines;
    newProcess->program = (char **) malloc(newProcess->numLines * sizeof(char *));

    for (int i = 0; i < newProcess->numLines; i++)
    {
        newProcess->program[i] = strdup(processToCopy->program[i]);
    }

    newProcess->memorySize = processToCopy->memorySize;
    newProcess->memory = (int *) malloc(newProcess->numLines * sizeof(int));

    for (int i = 0; i < newProcess->memorySize; i++)
    {
        newProcess->memory[i] = processToCopy->memory[i];
    }

    return newProcess;
}


void initializeProcessMemory(Process *process, int memorySize)
{
    process->memory = (Memory *) calloc(memorySize, sizeof(int));
    process->memorySize = memorySize;
}


void clearProcessMemory(Process *process, int position)
{
    process->memory[position] = 0;
}


void changeValueInProcessMemory(Process *process, int position, int value)
{
    process->memory[position] = value;
}


void increaseValueInProcessMemory(Process *process, int position, int value)
{
    process->memory[position] += value;
}


void reduceValueInProcessMemory(Process *process, int position, int value)
{
    process->memory[position] -= value;
}


void printProcessMemoryInfo(Process *process)
{
    printf("\nMemory:\n");

    for (int i = 0; i < process->memorySize; i++)
    {
        printf("%d ", process->memory[i]);
    }

    printf("\n");
}


void printProcessInfo(Process *process)
{
    printf("\nProgram:\n");

    for (int i = 0; i < process->numLines; i++)
    {
        printf("%s\n", process->program[i]);
    }

    printf("\n");
}


void freeProcess(Process *process)
{
    if (process)
    {
        if (process->program)
        {
            for (int i = 0; i < process->numLines; i++)
            {
                if (process->program[i])
                {
                    free(process->program[i]);
                }
            }
        }

        if (process->program)
        {
            free(process->program);
        }

        if (process->memory)
        {
            free(process->memory);
        }

        free(process);
    }
}


void reallocateProgram(Process *process, int newNumLines)
{
    if (process == NULL || process->program == NULL)
    {
        printf("Invalid process or program.\n");
        return;
    }

    process->program = realloc(process->program, newNumLines * sizeof(char *));
    if (process->program == NULL)
    {
        printf("Memory reallocation failed.\n");
        return;
    }

    process->numLines = newNumLines;
}