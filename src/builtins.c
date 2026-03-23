#include "builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int is_builtin(const char *cmd) {
    if (!strcmp(cmd, "exit") || !strcmp(cmd, "echo") || !strcmp(cmd, "type") || !strcmp(cmd, "pwd") || !strcmp(cmd, "cd"))
        return 1;
    return 0;
}

void exec_builtin(char **args, int parse_cnt) {
    if (!strcmp(args[0], "exit")) {
        exit(0);
    }
    else if (!strcmp(args[0], "echo")) {
        for (int i = 1; i < parse_cnt; i++) {
            printf("%s", args[i]);
            if(i < parse_cnt - 1) putchar(' ');
        }
        putchar('\n');
    }
    else if (!strcmp(args[0], "pwd")) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
            printf("%s\n", cwd);
    }
    else if (!strcmp(args[0], "cd")) {
        if(parse_cnt > 1) {
            if(!strcmp(args[1], "~")) {
                char *home = getenv("HOME");
                if(home) {
                    if(chdir(home))
                        printf("%s: %s: No such file or directory\n", args[0], home);
                }
                else printf("%s: HOME not set\n", args[0]);
            }
            else if(chdir(args[1]))
                printf("%s: %s: No such file or directory\n", args[0], args[1]);
        }
    }
    else if (!strcmp(args[0], "type")) {
        if (parse_cnt > 1){
            if (is_builtin(args[1])) {
                printf("%s is a shell builtin\n", args[1]);
            } else {
                char *path = getenv("PATH");
                if (!path) path = "";
                char *cpy_path = strdup(path);
                char *dir = strtok(cpy_path, ":");
                char full_path[1024];
                int found = 0;

                while (dir) {
                    snprintf(full_path, sizeof(full_path), "%s/%s", dir, args[1]);
                    if (!access(full_path, X_OK)) {
                        found = 1;
                        printf("%s is %s\n", args[1], full_path);
                        break;
                    }
                    dir = strtok(NULL, ":");
                }

                if (!found) {
                    printf("%s: not found\n", args[1]);
                }

                free(cpy_path);
            }
        }
    }
}