#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
int *cmds;
extern char** _args;
extern char** getline(void);
int pipe_count, redir_in, redir_out, pid,
    cmd_count, cmd_current, cmd_start, cmd_end;
extern int operror;
int debug = 1;

/*
 * Begin program.
 */
int piping(int pid, int* oldPipe, int* newPipe){
   return 0;
}
/* Fork and execute _args[cmds[z]] */
int fexec(int z, int* oldPipe, int* newPipe){
   pid = fork();
   piping(pid, oldPipe, newPipe);
   if(!pid) {
      /* In child */
      char *arg[] = { "echo", "Exec succeeded", NULL };
      int exec_fail = execvp("/bin/echo", arg);
      if(exec_fail == -1){
         fprintf(stderr, "Child: Execution of %s failed.\n", arg[0]);
         switch(errno){
            case ENOTDIR:
               fprintf(stderr, "\tA component of the path prefix is not a directory.\n"); break;
            case ENOENT:
               fprintf(stderr, "\tThe process file does not exist.\n"); break;
            case EFAULT:
               fprintf(stderr, "\tBad path, argv or envp.\n"); break;
            case E2BIG:
               fprintf(stderr, "\tArg list is too long.\n"); break;
            case ENOMEM:
               fprintf(stderr, "\tOut of memory.\n"); break;
            case EIO:
               fprintf(stderr, "\tIO error.\n"); break;
            case EACCES:
               fprintf(stderr, "\tPermission error or file is not +x.\n"); break;
         } exit(1);
      }
   }
   return pid;
}


int main(int argc, char *argv[], char *envp[]) {
   if(debug)
      printf("DEBUG ENABLED.\n");
   int i = 0;
   cmds = calloc(32, sizeof(int));
   _args = calloc(256, sizeof(char*));
   for(;;) {
      cmd_count = redir_in = redir_out = 
         i = cmd_current = cmd_start = cmd_end = 0;
      int oldPipe[2] = {0, 0};
      int newPipe[2] = {0, 0};
      printf(" dShell$ ");
      getline();
      if(_args[1] == NULL)
         continue;
      pipe_count = 0;  
      if(operror)
         continue;
      if(debug){
      for(i = 1; _args[i] != NULL; i++) {
         printf("Argument %d: %s\n", i, _args[i]);
      }
      for(i = 0; cmds[i] != -1; i++) {
         printf("cmds[%d]: %d\n", i, cmds[i]);
      }
      }
      /* 
       * Syntax parsing is complete. 
       * Now we need to do another parse up to each semicolon or newline
       * in order to prep for execution.
       */
      cmd_end = -1;
      while(cmd_start != cmd_end){
         /* -1 is EOL. -2 is ; */
         i = cmd_start + 1;
         while(i != cmd_end){
            /* Parse for operators in this phrase. */
            switch(cmds[i]){
               /* -10 is < */
               case -10: redir_in = 1; i+=2; break; 
               /* -11 is > */
               case -11: redir_out = 1; i+=2; break; 
               /* -12 is | */   
               case -12: pipe_count++; i+=2; break; 
               case -1:
               case -2:  cmd_end = i; break;
               /* Error! */
               default:  fprintf(stderr, "Unrecognized operator sigil.\n");
                         exit(1);
            }
         }
         if(debug){
         printf("redir_in:\t%d\n", redir_in);
         printf("redir_out:\t%d\n", redir_out);
         printf("pipe_count:\t%d\n", pipe_count);
         printf("cmd_end:\t%d\n", cmd_end);
         }
         /*
          * We should be ready to execute up to the first semicolon
          * or newline now; that position is noted by cmd_end.
          */
         /* For each command in the phrase... */ 
         for(cmd_current = cmd_start; cmd_current <= cmd_end; cmd_current+=2){
            /* Set up the file descriptors if necessary. */
            if(redir_in){
               if(debug)
                  printf("Generating redir_in descriptor.\n");
               /* TODO: Generate redir_in for first command. */
               redir_in--;
              cmd_current+=2;
            }
            if(redir_out && !pipe_count){
               if(debug)
                  printf("Generating redir_out descriptor.\n");
               redir_out--;
            }
            if(pipe_count){
               if(debug)
                  printf("Generating pipes.\n");
               /*if(piping(oldPipe, newPipe)){
                  fprintf(stderr, "Fatal piping error.\n");
                  exit(1);
               }*/
               --pipe_count;
            } else {
               /* TODO: Check redir_out condition, and fix stdout. */
            }
            if(debug)
               printf("fexecing.\n");
            /* TODO: fexec() */
           /* int return_status = 1;
		      pid = waitpid(pid,&return_status, 0);
		      if(debug)
		         printf("return status: %d\n", return_status);
		      if(return_status != 0)
		         fprintf(stderr, "Parent: Execution of a child failed - aborting execution.\n");
		
		         }
         int return_status;
         while(waitpid(-1, &return_status, 0) != -1){ }*/
         }
      if(cmds[cmd_end] == -2 && cmds[cmd_end] != -1)
         cmd_start = cmd_end++;
      else
         cmd_start = cmd_end;
      }
   }  
  return 0;
}
