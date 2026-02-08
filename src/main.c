#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  while(1){
    printf("$ ");
    char in[512];
    fgets(in, 512, stdin);
    in[strcspn(in, "\n")] = '\0';

    char *cpy_in = malloc((strlen(in) + 1) * sizeof(char)), *cmd_tok = NULL, *save = NULL;
    memcpy(cpy_in, in, strlen(in) + 1);
    cmd_tok = strtok_r(cpy_in, " \t", &save);

    if(!strcmp(cmd_tok, "exit")) { free(cpy_in); break; }
    else if(!strcmp(cmd_tok, "echo")) printf("%s", save);
    else if(!strcmp(cmd_tok, "type"))
      if(!strcmp(save, "exit") || !strcmp(save, "echo") || !strcmp(save, "type")) printf("%s is a shell builtin", save);
      else printf("%s: not found", save);
    else printf("%s: command not found", in);

    putchar('\n');
  }

  return 0;
}
