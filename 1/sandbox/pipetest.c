/* Author: Will Crawford <wacrawfo@ucsc.edu> */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main(int argc, char *argv[], char *envp[]) {
	int fd[2];
	printf("$ \n");
	if(pipe(fd)){ /* fd[0] = out end of the circular buffer, fd[1] = in side.*/
					/* Data written to fd[1] will be presented at fd[0]. */
		fprintf(stderr, "Pipe failed! How mysterious.\n");
		return 1;
	} 
	int pid = fork(); /* Forked processes inherit the file descriptor table of the parent. */
	if(pid){
		/* In parent. */
		close(fd[0]); /* Close the end of the pipe we aren't using for this process. */
		close(1); /* Close the file descriptor we're going to replace. OR use dup2. */
		dup(fd[1]); /* Duplicate fd[1] to the first available filehandle. (OR use dup2) */
		close(fd[1]); /* Close the file descriptor hanging off the end of our process's FD table. */
		printf("Talking through pipe - pipe test successful\n"); /* Pipe is ready on this end. */
	} else {
		/* In child. */
		close(fd[1]); /* Swap everything. */
		close(0);
		dup(fd[0]);
		close(fd[0]);
		int c;
		while((c = getchar()) != EOF)
			putchar(c);
	}
	return 0;
}

