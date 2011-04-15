#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

int cmds[32];
extern char** _args;
extern char **getline(void);
int pipe_count;
extern int operror;
int error_found;
int cmd_count;


int main(int argc, char *argv[], char *envp[]) {
  int i;
  int fd[2];
  cmd_count = 0;
  for(;;) {
    pipe_count = 0;
    fd[1] = 0;
    fd[2] = 0;
    printf(" dShell$ ");
    getline();
    cmds[cmd_count+1] = -1;
    if(operror)
      continue;
    for(i = 1; _args[i] != NULL; i++) {
      printf("Argument %d: %s\n", i, _args[i]);
    }
    printf("Arg loop complete.\n");
    fflush(stdout);
    for(i = 0; cmds[i] != -1; i++) {
      printf("cmds[%d]: %d\n", i, cmds[i]);
    }
    if(error_found)
      continue;
    /*for(i = 1;cmds[i] != -1; i+=2){
      while(strcmp(_args[cmds[i]], ";")){
         --i;
         
      }
    }*/
  }
  return 0;
}
