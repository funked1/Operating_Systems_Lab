/**
	Daniel Funke
	csc 345-01
	Lab 2, Exercise 3
**/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

int main(int argc, char** argv)
{
	pid_t pid;

	pid = fork();

	if (pid == 0) {
		// child process -- do nothing! be a zombie!
	}
	else if (pid > 0) {
		// parent process
		// we should *not* invoke wait
		//while (1);
		sleep(30);
	}

	return(0);
}
