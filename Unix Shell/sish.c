#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "history.h"
#include "cd.h"

#define MAXARGS 500
#define PROMPT "sish> "
#define SPC " "
// INPUTSIZE is the intial size of input buffer
#define INPUTSIZE 128

/*
 * The best shell program ever
 */
int main(int argc, char *argv[])
{
    char *output = PROMPT;
    char *arguments[MAXARGS];
    char *input = malloc(INPUTSIZE);
    size_t capacity = INPUTSIZE;
    bool inputFromHistory = false;

    historyClear();
    while (1) {
        if (!inputFromHistory) {
            printf("%s", output);
            int length = getline(&input, &capacity, stdin);
            if (length == -1) {
                perror("getline failed");
                exit(EXIT_FAILURE);
            }
            // Delete new line character
            input[strlen(input) - 1] = 0;
        }

        inputFromHistory = false;
        historyAdd(input);

        int argCount;
        char *str;
        char *saveptr;
        char *token;
        for (argCount = 0, str = input; ; argCount++, str = NULL) {
            token = strtok_r(str, SPC, &saveptr);
            if (token == NULL || argCount == MAXARGS) {
                // TODO: report error
                break;
            }
            arguments[argCount] = token;
        }


        if (strlen(arguments[0]) == 0) {
            continue;
        }
        if (strcmp(arguments[0], "exit") == 0) {
            // No need to free the getline malloc'd input buffer
            // The memory will be recovered by the OS when process exits
            exit(EXIT_SUCCESS);
        }
        if (strcmp(arguments[0], "history") == 0) {
            if (argCount == 1) {
            historyPrint();
            continue;
            }
            if (argCount == 2) {
                if (strcmp(arguments[1], "-c") == 0) {
                    historyClear();
                    continue;
                }
                long n = strtol(arguments[1], NULL, 10);
                if (n == 0 && errno == EINVAL) {
                    continue;
                }
                int nint = (int)n;
                // What if I call history of history?
                char *histSrc = historyGet(nint);
                strlcpy(input, histSrc, capacity);
                inputFromHistory = true;
                continue;
            }
        }
        if (strcmp(arguments[0], "cd") == 0) {
            cd(arguments[1]);
            continue;
        }
        if (strcmp(arguments[0], "pwd") == 0) {
            char cwd[1024];
            getcwd(cwd, sizeof(cwd));
            printf("%s\n", cwd);
        }
    }
}
