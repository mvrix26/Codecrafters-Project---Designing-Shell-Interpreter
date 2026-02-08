#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);
  char in[100];

  do{
    printf("$ ");

    fgets(in, 100, stdin);

    in[strlen(in) - 1] = '\0';
    if(strcmp(in, "exit")!=0)
      printf("%s: command not found\n", in);
  }while(strcmp(in, "exit")!=0);

  return 0;
}
