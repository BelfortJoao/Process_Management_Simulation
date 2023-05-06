#include <stdio.h>

#include "input.h"


void cleanStdin(void)
{
    int c = getchar();

    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}