#ifndef CODECRAFTERS_SHELL_REDIRECTOR_H
#define CODECRAFTERS_SHELL_REDIRECTOR_H

int setup_redirection(char **args, int *parse_cnt);
void restore_redirection(int saved_stdout);

#endif