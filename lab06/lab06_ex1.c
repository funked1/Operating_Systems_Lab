/* Dan Funke
   CSC345-01
   Lab 6 Exercise 1 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <wait.h>
#include <sys/types.h>
#include <semaphore.h>

/* Farmer */
#define MAX_WAIT 3

typedef struct _FARMER {
	pthread_t tid;
	int isNorth;
	int idx;
	int waitfor;
} FARMER;

/* Bridge */
sem_t bridge;

void enter_bridge(FARMER* f)
{
	if (f->isNorth == 1)
		printf("North farmer %d lined up the bridge.\n", f->idx);
	else
		printf("South farmer %d lined up the bridge.\n", f->idx);	

	sem_wait(&bridge);

	if (f->isNorth == 1)
		printf("North farmer %d just entered the bridge.\n", f->idx);
	else
		printf("South farmer %d just entered the bridge.\n", f->idx);

}

void exit_bridge(FARMER* f)
{
	if (f->isNorth == 1)
		printf("North farmer %d about to leave the bridge.\n", f->idx);
	else
		printf("South farmer %d about to leave the bridge.\n", f->idx);

	sem_post(&bridge);

	if (f->isNorth == 1)
		printf("North farmer %d just left the bridge.\n", f->idx);
	else
		printf("South farmer %d just left the bridge.\n", f->idx);

}

/* worker function */
void* pass_bridge(void* param)
{
	FARMER* f = (FARMER*)param;
	
	/* entry section */
	enter_bridge(f);

	/* critical section */
	if (f->isNorth == 1){
		printf("  North Farmer %d is passing bridge in %d seconds.\n",
				f->idx, f->waitfor);
	} else {
		printf("  South Farmer %d is passing bridge in %d seconds.\n",
				f->idx, f->waitfor);
	}

	sleep(f->waitfor);

	/* exit section */
	exit_bridge(f);

	pthread_exit(0);
}

int main (int argc, char** argv)
{
	sem_init(&bridge, 0, 1);

	int nNorth = atoi(argv[1]);
	int nSouth = atoi(argv[2]);

	/* create array of farmers  */
	FARMER* fNorth = (FARMER*)malloc(sizeof(FARMER) * nNorth);
	FARMER* fSouth = (FARMER*)malloc(sizeof(FARMER) * nSouth);

	/* prepare farmer information  */
	for (int i = 0; i < nNorth; ++i) {
		fNorth[i].isNorth = 1;
		fNorth[i].idx = i;
		fNorth[i].waitfor = rand() % MAX_WAIT;
	}
	for (int i = 0; i < nSouth; ++i) {
		fSouth[i].isNorth = 0;
		fSouth[i].idx = i;
		fSouth[i].waitfor = rand() % MAX_WAIT;
	}

	/* create threads  */
	for (int i = 0; i < nNorth; ++i) {
		pthread_create(&(fNorth[i].tid), NULL, pass_bridge, &(fNorth[i]));
	}
	for (int i = 0; i < nSouth; ++i) {
		pthread_create(&(fSouth[i].tid), NULL, pass_bridge, &(fSouth[i]));
	}

	/* wait for all farmers to pass the bridge */
	for (int i = 0; i < nNorth; ++i) {
		pthread_join(fNorth[i].tid, NULL);
	}
	for (int i = 0; i < nSouth; ++i) {
		pthread_join(fSouth[i].tid, NULL);
	}

	/* delete farmer arrays  */
	free(fNorth);
	free(fSouth);
	sem_destroy(&bridge);

	return 0;
}
