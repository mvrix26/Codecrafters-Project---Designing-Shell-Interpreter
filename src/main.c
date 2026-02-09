#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  while(1) {
    printf("$ ");
    char in[512];
    if(!fgets(in, 512, stdin)) break;
    in[strcspn(in, "\n")] = '\0';


    char *save = NULL;
    char *cpy_in = strdup(in);
    char *cmd_tok = strtok_r(cpy_in, " \t", &save);
    if(!cmd_tok) {
      free(cpy_in);
      continue;
    }


    if(!strcmp(cmd_tok, "exit")) {
      free(cpy_in);
      break;
    }
    else if(!strcmp(cmd_tok, "echo")) {
      char *arg = strtok_r(NULL, "", &save);
      if(arg) printf("%s", arg);
    }
    else if(!strcmp(cmd_tok, "type")) {
      char *arg = strtok_r(NULL, " \t", &save);
      if(!arg) {}
      else if(!strcmp(arg, "exit") || !strcmp(arg, "echo") || !strcmp(arg, "type")) {
        printf("%s is a shell builtin", arg);
      }
      else {
        char *path = getenv("PATH");
        char *cpy_path = strdup(path);
        char *dir = strtok(cpy_path, ":");
        char full_path[1024];
        int found = 0;

        while(dir) {
          snprintf(full_path, sizeof(full_path), "%s/%s", dir, arg);
          if(!access(full_path, X_OK)) {
            found = 1;
            printf("%s is %s", arg, full_path);
            break;
          }
          dir = strtok(NULL, ":");
        }

        if(!found) {
          printf("%s: command not found", arg);
        }

        free(cpy_path);
      }
    }
    else {
      printf("%s: command not found", in);
    }

    putchar('\n');
    free(cpy_in);
  }

  return 0;
}
