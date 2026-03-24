#ifndef CODECRAFTERS_SHELL_REDIRECTOR_H
#define CODECRAFTERS_SHELL_REDIRECTOR_H

int setup_redirection(char **args, int *parse_cnt, int *saved_stdout, int *saved_stderr);
void restore_redirection(int saved_stdout, int saved_stderr);

#endif