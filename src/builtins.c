#include "builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef enum {
    CMD_EXIT,
    CMD_ECHO,
    CMD_PWD,
    CMD_CD,
    CMD_TYPE,
    CMD_UNKNOWN
}CommandType;

static CommandType get_command_type(const char *cmd) {
    if (!strcmp(cmd, "exit")) return CMD_EXIT;
    if (!strcmp(cmd, "echo")) return CMD_ECHO;
    if (!strcmp(cmd, "pwd")) return CMD_PWD;
    if (!strcmp(cmd, "cd")) return CMD_CD;
    if (!strcmp(cmd, "type")) return CMD_TYPE;
    return CMD_UNKNOWN;
}

int is_builtin(const char *cmd) {
    return get_command_type(cmd) != CMD_UNKNOWN;
}

void exec_builtin(char **args, int parse_cnt) {
    switch (get_command_type(args[0])) {
            case CMD_EXIT:
            exit(0);
            break;

        case CMD_ECHO:
            for (int i = 1; i < parse_cnt; i++) {
                printf("%s", args[i]);
                if(i < parse_cnt - 1) putchar(' ');
            }
            putchar('\n');
            break;

        case CMD_PWD: {
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) != NULL)
                printf("%s\n", cwd);
            break;
        }

        case CMD_CD:
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
            break;

        case CMD_TYPE:
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
            break;

        default:
            break;
    }
}