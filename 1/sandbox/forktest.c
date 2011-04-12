/* Author: Will Crawford <wacrawfo@ucsc.edu> */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int *return_status;
int main(int argc, char *argv[], char *envp[]) {
	printf("$ \n");
	int pid = fork();
	if(pid){
		// In parent.
		printf("Waiting for process...\n");
		pid = wait(return_status);
		printf("Returned from process.\n");
	} else {
		// In child.
		printf("Child reporting.\n");
      printf("Fork test successful.\n");
	}
	return 0;
}
