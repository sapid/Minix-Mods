/* Author: Will Crawford <wacrawfo@ucsc.edu> */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int *return_status;
int main(int argc, char *argv[], char *envp[]) {
	printf("$ \n");
	int fp = open("./example.txt");
	int pid = fork();
	if(pid){
		/* In parent. */
		close(fp);
		wait(return_status);
	} else {
		/* In child. */
		close(0);
		dup(fp);
		int c;
		while((c = getchar()) != EOF)
			putchar(c);
	}
	return 0;
}
