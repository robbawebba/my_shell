#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "pipe.h"

// int main_pipe() {
// 	pipe_processes(test1, test2);
// 	return 0;
// }

void src(int pfd[], char ** cmd1, int contains_pipe)	{
	int pid;	/* we don't use the process ID here, but you may wnat to print it for debugging */
	// printf("%d contains type\n", contains_pipe);
	switch (pid = fork()) {
		case 0: /* child */
			close(1);

			//if |&
			if(contains_pipe==2)
			close(2);

			dup(pfd[1]);	/* this end of the pipe becomes the standard output */

			// if |&
			if(contains_pipe==2)
			dup(pfd[1]); //dup again

			close(pfd[0]); 		/* this process don't need the other end */
			execvp(cmd1[0], cmd1);	/* run the command */
			perror(cmd1[0]);	/* it failed! */

		default: /* parent does nothing */
			break;

		case -1:
			perror("fork");
			exit(1);
	}
}

void rundest(int pfd[], char ** cmd2) {
	int pid;

	switch (pid = fork()) {
		case 0: /* child */
			close(0);
			dup(pfd[0]);	/* this end of the pipe becomes the standard input */
			close(pfd[1]);		/* this process doesn't need the other end */
			execvp(cmd2[0], cmd2);	/* run the command */
			perror(cmd2[0]);	/* it failed! */

		default: /* parent does nothing */
			break;

		case -1:
			perror("fork");
			exit(1);
	}
}

int pipe_processes(char ** cmd1, char ** cmd2, int contains_pipe) {
	int pid, status;
	int fd[2];
	// printf("contains pipe %d\n", contains_pipe);
	// printf("cmd1[0]: %s\n", cmd1[0]);
	// printf("cmd2[0]: %s \n", cmd2[0]);

	if (pipe(fd) == -1) { //get file descriptors
    perror("pipe");
    exit(1);
  }

	src(fd, cmd1, contains_pipe);
	rundest(fd, cmd2);
	close(fd[0]); close(fd[1]); 	/* this is important! close both file descriptors on the pipe */

	while ((pid = wait(&status)) != -1)	/* pick up all the dead children */
		fprintf(stderr, "process %d exits with %d\n", pid, WEXITSTATUS(status));
	return 0;
}
