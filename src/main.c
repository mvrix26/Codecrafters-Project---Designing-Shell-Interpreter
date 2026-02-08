#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  while(1){
    printf("$ ");

    char in[100];
    fgets(in, 100, stdin);

    in[strlen(in) - 1] = '\0';
    printf("%s: command not found\n", in);
  }

  return 0;
}
