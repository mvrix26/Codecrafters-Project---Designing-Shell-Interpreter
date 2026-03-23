#ifndef CODECRAFTERS_SHELL_BUILTINS_H
#define CODECRAFTERS_SHELL_BUILTINS_H

int is_builtin(const char *cmd);
void exec_builtin(char **args, int parse_cnt);

#endif