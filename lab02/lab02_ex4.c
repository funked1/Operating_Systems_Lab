/**
	Daniel Funke
	CSC345-01
	Lab 02, Exercise 4
**/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <wait.h>
#include <string.h>

int main(int argc, char** argv)
{
	const char *name = "/shmem1";
	const int SIZE = 4096;
	int n = atoi(argv[1]);
	char snum[5];
	pid_t id = fork();

	int shm_fd;
	void *ptr;
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, SIZE);
	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

	if (n <= 0){
		printf("Please enter a positive integer value for n");
		return 0;
	}

	if (id == 0){
		while (n > 1){
			sprintf(snum, "%d, ", n);
			sprintf(ptr, "%s", snum);
			ptr += strlen(snum);
			if ((n % 2) == 0){
				n = n / 2;
			}
			else {
				n = (3 * n) + 1;
			}
		}
		sprintf(snum, "%d\n", n);
		sprintf(ptr, "%s", snum);
		ptr += strlen(snum);
	}
	else {
		wait(NULL);
		printf("%s", (char *)ptr);
		shm_unlink(name);
	}

	return 0;
}
