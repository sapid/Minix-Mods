#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int *cmds;
extern char** _args;
extern char** getline(void);
int pipe_count, error_found, cmd_count, 
    redir_in, redir_out, clause_count, next_cmd;
extern int operror;



int main(int argc, char *argv[], char *envp[]) {
  int i, j, k = 0;
  cmds = calloc(32, sizeof(int));
  _args = calloc(256, sizeof(char*));
    for(;;) {
      cmd_count = redir_in = redir_out = 
         error_found = clause_count = next_cmd = 0;
      int oldPipe[2] = {0, 0};
      int newPipe[2] = {0, 0};
      printf(" dShell$ ");
      getline();
      pipe_count = 0;
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
      /* -1 is EOl. -2 is ; */
      for(i = 1;cmds[i] != -1 || cmds[i] != -2; i+=2){ 
         switch(cmds[i]){
            case -10: /* -10 is < */
               redir_in = 1;
               break; /* -11 is > */
            case -11:
               redir_out = 1;
               break;
            case -12: /* -12 is | */
               pipe_count++;
               break;
         }
         
         }
      }
  }
  return 0;
}
