#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

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
      if(arg) printf("%s\n", arg);
    }
    else if(!strcmp(cmd_tok, "pwd")) {
      char cwd[1024];
      getcwd(cwd, sizeof(cwd));
      printf("%s\n", cwd);
    }
    else if(!strcmp(cmd_tok, "type")) {
      char *arg = strtok_r(NULL, " \t", &save);
      if(!arg) {}
      else if(!strcmp(arg, "exit") || !strcmp(arg, "echo") || !strcmp(arg, "type") ||!strcmp(arg, "pwd")) {
        printf("%s is a shell builtin\n", arg);
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
            printf("%s is %s\n", arg, full_path);
            break;
          }
          dir = strtok(NULL, ":");
        }

        if(!found) {
          printf("%s: not found\n", arg);
        }

        free(cpy_path);
      }
    }
    else {
      char *path = getenv("PATH");
      char *cpy_path = strdup(path);
      char *dir = strtok(cpy_path, ":");
      char full_path[1024];
      int found = 0;

      while(dir) {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd_tok);
        if(!access(full_path, X_OK)){
          char *args[64] = {NULL};
          char *arg = strtok_r(NULL, " \t", &save);
          args[0] = cmd_tok;
          int i = 1;
          while(arg) {
            args[i++] = arg;
            arg = strtok_r(NULL, " \t", &save);
          }
          args[i] = NULL;

          found = 1;
          pid_t pid = fork();
          if(!pid){
            execv(full_path, args);
          }
          else {
            wait(NULL);
            break;
          }
        }
        dir = strtok(NULL, ":");
      }

      free(cpy_path);
      if(!found){
        printf("%s: command not found\n", cmd_tok);
      }
    }

    free(cpy_in);
  }

  return 0;
}
