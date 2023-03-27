#include <stdio.h>
#include <stdlib.h>

#include "../interface/lexical-analyzer.h"

int main()
{
    readFile("test.c");

    do
    {
    } while (getNextToken() != END);

    printf("showTokens:\n");
    showAtoms();
    return 0;
}