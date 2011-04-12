/* Author: Will Crawford <wacrawfo@ucsc.edu> */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int *return_status;
int errno;
void execute(void){
   int pid = fork();
   if(pid){
      /* In parent */
      pid = waitpid(pid,return_status);
      if(&return_status)
         fprintf(stderr, "Parent: Execution of child failed.\n");
      printf("Process died.\n");
   } else {
      /* In child */
      char *arg[] = { 0 };
      int exec_fail = execvp("ls", arg);
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
