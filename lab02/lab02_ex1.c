/**
	Daniel Funke
	CSC345-01
	Lab 2, Exercise 1
 **/

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int value = 5;

int main() {
	pid_t pid;
	pid = fork();
	if(pid == 0) {
		value += 15;
		return 0;
	}
	else if (pid > 0){
		wait(NULL);
		printf("PARENT: value = %d", value); 
		return 0;
	}
}
