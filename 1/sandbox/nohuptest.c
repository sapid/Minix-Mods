/* Author: Will Crawford <wacrawfo@ucsc.edu> */
/* Don't have to do this. Lulz. */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int *return_status;

void execute(){

}

void nohup(){
   int pid = fork();
   if(pid){
      /* In parent. */
   } else {
      /* In child. */
      close(0);
   }

}
int main(int argc, char *argv[], char *envp[]) {
   printf("$ \n");
   nohup();
   return 0;
}
