/* Author: Will Crawford <wacrawfo@ucsc.edu> */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

extern char **getline();

int main(int argc, char *argv[], char *envp[]) {
  int i;
  char **args; 

  while(1) {
    printf("$ ");
    args = getline();
	 parse_args(args);
    for(i = 0; args[i] != NULL; i++) {
      printf("Argument %d: %s\n", i, args[i]);
    }
  }
}
