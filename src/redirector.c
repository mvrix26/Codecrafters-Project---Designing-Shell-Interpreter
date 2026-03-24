#include "redirector.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int setup_redirection(char **args, int *parse_cnt, int *saved_stdout, int *saved_stderr) {
    char *outfile = NULL;
    char *errfile = NULL;

    for (int i = *parse_cnt - 1; i >= 0; i--) {
        if (!strcmp(args[i], "2>")) {
            errfile = args[i + 1];
            args[i] = NULL;
            *parse_cnt = i;
        } else if (!strcmp(args[i], "1>") || !strcmp(args[i], ">")) {
            outfile = args[i + 1];
            args[i] = NULL;
            *parse_cnt = i;
        }
    }

    if (outfile != NULL) {
        *saved_stdout = dup(STDOUT_FILENO);
        int fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);

        if (fd < 0) {
            printf("bash: %s permission denied\n", outfile);
            close(*saved_stdout);
            *saved_stdout = -1;
            return -1;
        }

        dup2(fd, STDOUT_FILENO);
        close(fd);
    }

    if (errfile != NULL) {
        *saved_stderr = dup(STDERR_FILENO);
        int fd = open(errfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);

        if (fd < 0) {
            printf("bash: %s permission denied\n", errfile);
            close(*saved_stderr);
            *saved_stderr = -1;
            return -1;
        }

        dup2(fd, STDERR_FILENO);
        close(fd);
    }

    return 0;
}

void restore_redirection(int saved_stdout, int saved_stderr) {
    if (saved_stdout != -1) {
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
    }

    if (saved_stderr != -1) {
        dup2(saved_stderr, STDERR_FILENO);
        close(saved_stderr);
    }
}