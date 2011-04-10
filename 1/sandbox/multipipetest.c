/* Author: Will Crawford <wacrawfo@ucsc.edu> */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int *return_status;
void pipe_child(int *oldPipe, int *newPipe){
	pipe(newPipe);
	pid = fork();
	if(pid){
		// In parent. Shuffle the pipes for the next child.
		oldPipe[0] = newPipe[0]; 
		oldPipe[1] = newPipe[1];
		return;
	}
	else {
		// In child.
		close(newPipe[0]);
		close(0);
		dup(oldPipe[0]);
		close(1);
		dup(newPipe[1]);
		// File descriptor exchange complete. Exec!
		return;
	}
}
void last_child(int *oldPipe, int *newPipe){
	pid = fork();
	if(pid){
		// In parent.
		return;
	}
	else {
		close(newPipe[0]);
		close(0);
		dup(oldPipe[0]);
		close(1);
		dup(newPipe[1]);
		return;
	}
}
int main(int argc, char *argv[], char *envp[]) {
	int oldPipe[2] = {0, 1};
	int newPipe[2];
	printf("$ \n");
	if(pipe(fd)){
		fprintf(stderr, "Pipe failed! How mysterious.\n");
		return 1;
	}
	int pid;
	// For the pipe test, the parent will feed the child.
	// In live shell usage, this next line won't exist.
	oldPipe[0] = 1;
	for(int i; i < 4; i++){
		if(pid)
			pipe_child(oldPipe, newPipe);
	}

	if(pid){
		wait(return_status);
	}
	return 0;
}
