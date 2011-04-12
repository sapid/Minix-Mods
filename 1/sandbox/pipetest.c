/* Author: Will Crawford <wacrawfo@ucsc.edu> */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main(int argc, char *argv[], char *envp[]) {
	int fd[2];
	printf("$ \n");
	if(pipe(fd)){
		fprintf(stderr, "Pipe failed! How mysterious.\n");
		return 1;
	}
	int pid = fork();
	if(pid){
		// In parent.
		close(fd[0]);
		close(1);
		dup(fd[1]);
		close(fd[1]);
		printf("Talking through pipe - pipe test successful\n");
	} else {
		// In child.
		close(fd[1]);
		close(0);
		dup(fd[0]);
		close(fd[0]);
		int c;
		while((c = getchar()) != EOF)
			putchar(c);
	}
	return 0;
}
