#include <stdio.h>
#include <unistd.h>

void cd(const char *filePath) {
    int result = chdir(filePath);
    if (result == -1) {
        printf("Invalid filepath");
    }
}
