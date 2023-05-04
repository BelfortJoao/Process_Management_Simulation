#include <stdio.h>

#include "input.h"


/*
 *  Clears stdin to avoid problems with the "scanf()" function.
 */
void cleanStdin(void)
{
    int c = getchar();

    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}