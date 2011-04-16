#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <fcntl.h>
int *cmds;
extern char** _args;
extern char** getline(void);
int pipe_count, redir_in, redir_out, pid, 
    cmd_count, cmd_current, cmd_start, cmd_end;
extern int operror;
int debug = 1;
int exec_failed;
int return_status;

/*
 * Begin program.
 */
int piping(int pid, int* oldPipe, int* newPipe){
   if(pid){
      /* In parent.*/
      if(debug)
         printf("oldPipe: %d, %d\tnewPipe: %d, %d\n", oldPipe[0], oldPipe[1], newPipe[0], newPipe[1]);
      if(oldPipe[0]>=0) close(oldPipe[0]); /* Failure here means doing nothing, so we don't listen for a return. */
      oldPipe[0] = newPipe[0];
      if(oldPipe[1]>=0) close(oldPipe[1]); 
      oldPipe[1] = newPipe[1];
      if(debug) printf("%d Swap complete.\n", pid);
      }
   else{
      /* In child. */
      if(debug) printf("A child is swapping pipes.\n");
      if(oldPipe[0]>=0){
         if(debug) printf("Swapping input... %d\n", oldPipe[0]);
         close(newPipe[0]);/* Failure here means doing nothing, so we don't listen for a return. */
         close(0);
         if(dup(oldPipe[0]) < 0)
            fprintf(stderr, "1 Duping a pipe failed in a child: %d\n", errno);
         close(oldPipe[0]);
      }
      if(newPipe[1]>=0){
         if(debug) printf("Swapping output... duping %d\n", newPipe[1]);
         close(1);
         if(dup(newPipe[1]) < 0)
            fprintf(stderr, "2 Duping a pipe failed in a child: %d\n", errno);
         close(newPipe[1]);
      if(newPipe[0]>=0)
         close(newPipe[0]);
      if(oldPipe[1]>=0)
         close(oldPipe[1]);
      }
   }
   return 0;
}
/* Fork and execute _args[cmds[z]] */
int fexec(int z, int* oldPipe, int* newPipe){
   pid = fork();
   piping(pid, oldPipe, newPipe);
   if(!pid) {
      /* In child */
      execvp(_args[cmds[z]], &_args[cmds[z]]);
      fprintf(stderr, "Child: Execution of %s failed.\n", _args[cmds[z]]);
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
         default:
            fprintf(stderr, "\tMisc error.\n"); break;
      } exit(1);
   }
   return pid;
}


int main(int argc, char *argv[], char *envp[]) {
   int oldPipe[2];
   int newPipe[2];
   int i = 0;
   if(debug) printf("DEBUG ENABLED.\n");
   cmds = calloc(32, sizeof(int));
   _args = calloc(256, sizeof(char*));
   while(1){
      oldPipe[0] = -1;
	   oldPipe[1] = -1;
      newPipe[0] = -1;
	   newPipe[1] = -1;
      cmd_count = redir_in = redir_out = 
         i = cmd_current = cmd_start = cmd_end = 0;
      printf(" dShell$ ");
      getline();
      if(_args[1] == NULL)
         continue;
      pipe_count = 0;  
      if(operror)
         continue;
      if(debug){
      for(i = 0; cmds[i] != -1; i++) {
         printf("cmds[%d]: ", i);
         if(cmds[i]>-1)
            printf("_args[%d]: %s\n", cmds[i], _args[cmds[i]]);
         else
            printf("%d\n", cmds[i]);
      }
      }
      /* 
       * Syntax parsing is complete. 
       * Now we need to do another parse up to each semicolon or newline
       * in order to prep for execution.
       * A note about cmds array structure:
       *    Even indices are command name or file name indices in _args.
       *    Odd indices are operators (or the end of the line).
       */
      i = 0;
      cmd_end = -1;
      while(cmd_start != cmd_end){
         if(debug)
         while(cmds[cmd_start] == -2){
            cmd_start++;}
         if(cmds[cmd_start] == -1){
            if(debug)
               printf("No input detected.\n");
            break;
         }
         /* -1 is EOL. -2 is ; */
         i = cmd_start + 1;
         if(cmds[i] == -1)
            cmd_end = i;
         while(i != cmd_end){
            if(debug)
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
         /* TODO: Validate that commands and files exist? */
         if(debug){
         printf("redir_in:\t%d", redir_in);
         printf("\tredir_out:\t%d", redir_out);
         printf("\tpipe_count:\t%d\n", pipe_count);
         printf("cmd_indices:\t%d, %d\n", cmd_start, cmd_end);
         }
         /*
          * We should be ready to execute up to the first semicolon
          * or newline now; that position is noted by cmd_end.
          */
         /* For each command in the phrase... */
         exec_failed = 0;
         for(cmd_current = cmd_start; cmd_current <= cmd_end; cmd_current+=2){
            int cmd_push = 0;
            int return_status = 0;
            int open_failed = 0;
            int pipe_failed = 0;
            /* Set up the file descriptors if necessary. */
            if(redir_in){
               if(debug)
                  printf("Generating redir_in descriptor to %s.\n", _args[cmds[cmd_current+2]]);
               if(oldPipe[0] > -1) close(oldPipe[0]);
               oldPipe[0] = open(_args[cmds[cmd_current+2]], O_RDONLY);
               if(oldPipe[0] < 0)
                  open_failed++;
               redir_in--;
               cmd_push+=2;
            }
            if(!redir_out && !pipe_count){
               if(newPipe[1] > -1) close(newPipe[1]);
               if(newPipe[0] > -1) close(newPipe[0]);
               newPipe[1] = -1; newPipe[0] = -1;
            }
            if(redir_out && !pipe_count){
               if(debug)
                  printf("Generating redir_out descriptor to %s.\n", _args[cmds[cmd_current+2]]);
               if(newPipe[1] > -1) close(newPipe[1]);
               newPipe[1] = open(_args[cmds[cmd_current+2]], O_CREAT | O_WRONLY, 0755);
               if(newPipe[1] < 0)
                  open_failed++;
               redir_out--;
               cmd_push +=2;
            }
            if(pipe_count){
               if(debug) printf("Generating pipes.\n");
               if(pipe(newPipe))
                  pipe_failed++;
               --pipe_count;
            } 
            if(open_failed){
               fprintf(stderr, "Opening file %s failed. Did you forget to include the path? \n\tNot executing %s or further. Beware of broken pipes.\n", _args[cmds[cmd_current+2]], _args[cmds[cmd_current]]);
               exec_failed++;
               break;
            }
            if(pipe_failed){
               fprintf(stderr, "Opening a pipe for %s failed. Halting further execution and resetting shell. \n\tBeware of broken pipes.\n", _args[cmds[cmd_current]]);
               exec_failed++;
               break;
            }
            if(debug) printf("fexecing %s.\n", _args[cmds[cmd_current]]);
            fexec(cmd_current, oldPipe, newPipe); 
            if(cmd_push)
               cmd_current += cmd_push;
         }
      return_status = 0;
      if(exec_failed)
         fprintf(stderr, "Something went wrong. Waiting for open children...\n");
      if(newPipe[0]>=0) close(newPipe[0]);
      if(newPipe[1]>=0) close(newPipe[1]);
      if(oldPipe[0]>=0) close(oldPipe[0]);
      if(oldPipe[1]>=0) close(oldPipe[1]);
      while(printf("Waiting...\n") && waitpid(-1, &return_status, 0) != -1){
         if(debug) printf("A child returned with status %d\n", return_status);
         if(return_status)
            fprintf(stderr, "Parent: A child returned an exit code indicating failure.\n");
      }
      if(exec_failed)
         break;
      if(cmds[cmd_end] == -2 && cmds[cmd_end+1] != -1)
         cmd_start = cmd_end++;
      else
         cmd_start = cmd_end;
      }
   }  
  return 0;
}
