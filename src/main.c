#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int parse(char *in, char **args) {
  int argc = 0;
  int sq_flag = 0, dq_flag = 0;
  char *r = in;

  while(*r == ' ' || *r == '\t') r++;
  if(*r == '\0') return 0;

  args[argc++] = r;
  char *w = r;

  while(*r != '\0') {
    if(*r == '\'') {
      if(dq_flag) {
        *w = *r;
        w++;
      }
      else
        sq_flag = !sq_flag;
    }
    else if(*r == '\"') {
      if(sq_flag) {
        *w = *r;
        w++;
      }
      else
        dq_flag = !dq_flag;
    }
    else if((*r == ' ' || *r == '\t') && !sq_flag && !dq_flag) {
      *w = '\0';
      w++;
      while(*(r+1) == ' ' || *(r+1) == '\t')
        r++;
      if(*(r+1) != '\0')
        args[argc++] = w;
    }
    else {
      *w = *r;
      w++;
    }
    r++;
  }

  *w = '\0';
  args[argc] = NULL;

  return argc;
}

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

    if(!strcmp(args[0], "exit")) {
      break;
    }
    else if(!strcmp(args[0], "echo")) {
      for(int i = 1; i < parse_cnt; i++) {
        printf("%s", args[i]);
        if(i < parse_cnt - 1) putchar(' ');
      }
      putchar('\n');
    }
    else if(!strcmp(args[0], "pwd")) {
      char cwd[1024];
      getcwd(cwd, sizeof(cwd));
      printf("%s\n", cwd);
    }
    else if(!strcmp(args[0], "cd")) {
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
    }
    else if(!strcmp(args[0], "type")) {
      if(parse_cnt > 1){
        if(!strcmp(args[1], "exit") || !strcmp(args[1], "echo") || !strcmp(args[1], "type") ||!strcmp(args[1], "pwd") || !strcmp(args[1], "cd")) {
          printf("%s is a shell builtin\n", args[1]);
        } else {
          char *path = getenv("PATH");
          char *cpy_path = strdup(path);
          char *dir = strtok(cpy_path, ":");
          char full_path[1024];
          int found = 0;

          while(dir) {
            snprintf(full_path, sizeof(full_path), "%s/%s", dir, args[1]);
            if(!access(full_path, X_OK)) {
              found = 1;
              printf("%s is %s\n", args[1], full_path);
              break;
            }
            dir = strtok(NULL, ":");
          }

          if(!found) {
            printf("%s: not found\n", args[1]);
          }

          free(cpy_path);
        }
      }
    }
    else {
      char *path = getenv("PATH");
      char *cpy_path = strdup(path);
      char *dir = strtok(cpy_path, ":");
      char full_path[1024];
      int found = 0;

      while(dir) {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, args[0]);
        if(!access(full_path, X_OK)){
          found = 1;
          pid_t pid = fork();
          if(!pid){
            execv(full_path, args);
            exit(1);
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
        printf("%s: command not found\n", args[0]);
      }
    }
  }

  return 0;
}
