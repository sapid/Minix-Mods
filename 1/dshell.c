#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

extern int *cmds;
extern char **getline(void);
int pipe_count;
extern int operror;
int error_found;
extern int cmdcount;


int main(int argc, char *argv[], char *envp[]) {
  int i;
  char **args; 

  for(;;) {
    pipe_count = 0;
    printf(" dShell$ ");
    args = getline();
    if(operror)
      continue;
    for(i = 1; args[i] != NULL; i++) {
      printf("Argument %d: %s\n", i, args[i]);
    }
    printf("Arg loop complete.\n");
    fflush(stdout);
    for(i = 0; cmds[i] != -1; i++) {
      printf("cmds[%d]: %d\n", i, cmds[i]);
    }
    if(error_found)
      continue;
  }
  return 0;
}
