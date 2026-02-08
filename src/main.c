#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);
  char in[100];

  while(1){
    printf("$ ");
    fgets(in, 100, stdin);
    in[strcspn(in, "\n")] = '\0';

    char *ec = strstr(in, "echo");
    if(ec)
      if(ec == in){
        printf("%s\n", in + 5);
        continue;
      }

    if(!strcmp(in, "exit"))
      break;

    printf("%s: command not found\n", in);
  }

  return 0;
}
