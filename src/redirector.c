#include "redirector.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int setup_redirection(char **args, int *parse_cnt) {
    int saved_stdout = -1;
    char *outfile = NULL;

    for (int i = 0; i < *parse_cnt; i++) {
        if (!strcmp(args[i], "1>") || !strcmp(args[i], ">")) {
            outfile = args[i + 1];
            args[i] = NULL;
            *parse_cnt = i;
            break;
        }
    }

    if (outfile != NULL) {
        saved_stdout = dup(STDOUT_FILENO);
        int fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);

        if (fd < 0) {
            printf("bash: %s permission denied\n", outfile);
            close(saved_stdout);
            return -2;
        }

        dup2(fd, STDOUT_FILENO);
        close(fd);
    }

    return saved_stdout;
}

void restore_redirection(int saved_stdout) {
    if (saved_stdout != -1) {
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
    }
}