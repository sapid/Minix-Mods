/* Author: Will Crawford <wacrawfo@ucsc.edu> */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int *return_status;
int pid;
void pipe_child(int *oldPipe, int *newPipe){
   pipe(newPipe);
   pid = fork();
   if(pid){
      // In parent. Shuffle the pipes for the next child.
      close(oldPipe[0]);
      oldPipe[0] = newPipe[0]; 
      close(oldPipe[1]);
      oldPipe[1] = newPipe[1];
      return;
   }
   else {
      // In child.
      if(oldPipe[0]){
         close(newPipe[0]);
         close(0);
         dup(oldPipe[0]);
         close(oldPipe[0]);
      }
      if(newPipe[0]){
         close(1);
         dup(newPipe[1]);
         close(newPipe[1]);
      }
      // File descriptor exchange complete. Exec!
      return;
   }
}
int main(int argc, char *argv[], char *envp[]) {
   int oldPipe[2]; 
   int newPipe[2];
   printf("$ \n");
   // For the pipe test, the parent will feed the child.
   // In live shell usage, this next line won't exist.
   pid = 1;
   int i;
   for(i = 0; i < 4; i++){
      if(pid)
         pipe(newPipe);
         pipe_child(oldPipe, newPipe);
   }
   newPipe[0] = 0;
   newPipe[1] = 0;
   pipe_child(oldPipe, newPipe); // This child should print to stdout.
   if(pid)
      printf("Multi-pipe test successful.\n");
   if(pid){
      wait(return_status);
   }
   return 0;
}
