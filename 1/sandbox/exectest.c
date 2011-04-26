/* Author: Will Crawford <wacrawfo@ucsc.edu> */
#include <stdio.h>
#include <sys/types.h> /* For wait() */
#include <sys/wait.h>  /* For wait() */
#include <unistd.h>
#include <stdlib.h>
int errno;
void execute(void){
   int pid = fork();
   if(pid){
      /* In parent */
      int return_status = 1;
      pid = waitpid(pid,&return_status, 0);
      printf("return status: %d\n", return_status);
      if(return_status = 0)
         fprintf(stderr, "Parent: Execution of child failed.\n");
      printf("Process died.\n");
   } else {
      /* In child */
      char *arg[] = { "echo", "Exec succeeded", NULL };
      int exec_fail = execvp("/bin/echo", arg);
      if(exec_fail == -1){
         fprintf(stderr, "Child: Execution of %s failed with errno %d.\n", arg[0], errno);
         exit(1);
      }

   }
}

int main(int argc, char *argv[], char *envp[]) {
	printf("$ \n");
   char **arg[2][5];
   execute();
	return 0;
}
