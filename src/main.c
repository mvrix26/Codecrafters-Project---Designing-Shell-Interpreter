#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "builtins.h"
#include "executor.h"

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  while(1) {
    printf("$ ");
    char in[512];
    if(!fgets(in, 512, stdin)) break;
    in[strcspn(in, "\n")] = '\0';

    char *args[64];
    int parse_cnt = parse(in, args);
    if(!parse_cnt) continue;

    if (is_builtin(args[0])) {
      exec_builtin(args, parse_cnt);
    } else {
      exec_external(args);
    }
  }

  return 0;
}