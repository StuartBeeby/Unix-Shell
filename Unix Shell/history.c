#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "history.h"

int histCount = 0;
char *histArr[MAXHIST];

void historyAdd(char *input) {
    size_t length = strlen(input);
    char *buffer = malloc(length + 1);
    strlcpy(buffer, input, length + 1);
    if (histCount >= MAXHIST) {
        free(histArr[0]);
        for (int i = 1; i < MAXHIST; i++) {
            histArr[i-1] = histArr[i];
        }
        histCount--;
    }
    histArr[histCount] = buffer;
    histCount++;
}

void historyClear()
{
    for (int i = 0; i < histCount; i++) {
        free(histArr[i]);
    }
    histCount = 0;
}

void historyPrint()
{
    for (int i = 0; i < histCount; i++) {
        printf("%d %s\n", i, histArr[i]);
    }
}

char *historyGet(int number) {
    if (number < 0 || number > histCount - 1) {
        return "";
    }
    return histArr[number];
}
