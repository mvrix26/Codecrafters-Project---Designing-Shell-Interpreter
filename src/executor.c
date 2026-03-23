#include "executor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void exec_external(char **args) {
    char *path = getenv("PATH");
    if (!path) path = "";
    char *cpy_path = strdup(path);
    char *dir = strtok(cpy_path, ":");
    char full_path[1024];
    int found = 0;

    while (dir) {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, args[0]);
        if (!access(full_path, X_OK)){
            found = 1;
            pid_t pid = fork();
            if (!pid){
                execv(full_path, args);
                exit(1);
            }
            wait(NULL);
            break;
        }
        dir = strtok(NULL, ":");
    }

    free(cpy_path);
    if (!found){
        printf("%s: command not found\n", args[0]);
    }
}