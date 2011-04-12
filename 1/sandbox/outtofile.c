/* Author: Will Crawford <wacrawfo@ucsc.edu> */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
int *return_status;
int main(int argc, char *argv[], char *envp[]) {
	printf("$ \n");
	int fp = open("./example-out.txt", O_CREAT | O_WRONLY, 0755);
	int pid = fork();
	if(pid){
		// In parent.
		close(fp);
		wait(return_status);
	} else {
		// In child.
		close(1);
		dup(fp);
		close(fp);
		int c;
		printf("Test successful.\n");
	}
	return 0;
}
