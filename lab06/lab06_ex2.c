/* Dan Funke
   CSC345-01
   Lab 6 Exercise 2 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <wait.h>

#define MAX_WAIT 3

typedef struct _FARMER {
	pthread_t tid;
	int isNorth;
	int idx;
	int waitfor;
} FARMER;


/* Bridge */
pthread_mutex_t c_mutex;
pthread_cond_t  c_condv;

int num_waiting_north = 0; // to denote how many are currently waiting at north-end
int num_waiting_south = 0; // to denote how many are currently waiting at south-end
int on_bridge = 0; // to indicate if anyone is currently crossing the bridge
int prev = 0; // to indicate whose turn it was before

void enter_bridge_north(FARMER* f)
{
	++num_waiting_north;	
	printf("North farmer %d lined up on the bridge.\n", f->idx);
	
	pthread_mutex_lock(&c_mutex);
	while (on_bridge == 1 || prev == 0)
		pthread_cond_wait(&c_condv, &c_mutex);

	--num_waiting_north;
	prev = 0;
	on_bridge = 1;
	printf("North farmer %d just entered the bridge.\n", f->idx);
}

void exit_bridge_north(FARMER* f)
{
	on_bridge = 0;
	printf("North farmer %d about to leave the bridge.\n", f->idx);

	// notify everyone waiting to pass the bridge (regardless of direction)
	pthread_mutex_unlock(&c_mutex);
	pthread_cond_broadcast(&c_condv);

	printf("North farmer %d just left the bridge.\n", f->idx);
}

void enter_bridge_south(FARMER* f)
{
	++num_waiting_south;
	printf("South farmer %d lined up on the bridge.\n", f->idx);

	pthread_mutex_lock(&c_mutex);
	while (on_bridge == 1 || prev == 1)
		pthread_cond_wait(&c_condv, &c_mutex);

	--num_waiting_south;
	prev = 1;
	on_bridge = 1;
	printf("South farmer %d just entered the bridge.\n", f->idx);
}

void exit_bridge_south(FARMER* f)
{
	on_bridge = 0;
	printf("South farmer %d about to leave the bridge.\n", f->idx);

	// notify everyone waiting to pass the bridge (regardless of direction)
	pthread_mutex_unlock(&c_mutex);
	pthread_cond_broadcast(&c_condv);

	printf("South farmer %d just left the bridge.\n", f->idx);
}

void* pass_bridge(void* param)
{
	FARMER* f = (FARMER*)param;

	/* Entry Section */
	if (f->isNorth) {
		enter_bridge_north(f);
	} else {
		enter_bridge_south(f);
	}

	/* Critical Section */
	if (f->isNorth == 1) {
		printf("   North Farmer %d is passing bridge in %d seconds.\n",
				f->idx, f->waitfor);
	} else {
		printf("   South Farmer %d is passing bridge in %d seconds.\n",
				f->idx, f->waitfor);
	}
	sleep(f->waitfor);

	/* Exit Section */	
	if (f->isNorth) {
		exit_bridge_north(f);
	} else {
		exit_bridge_south(f);
	}

	pthread_exit(0);
}

int main (int argc, char** argv)
{
	int nNorth = atoi(argv[1]);
	int nSouth = atoi(argv[2]);

	/* Initialize Mutex Lock */
	pthread_mutex_init(&c_mutex, NULL);

	/* Initialize Conditional Variable */
	pthread_cond_init(&c_condv, NULL);

	/* Create an Array of Farmers */
	FARMER* fNorth = (FARMER*)malloc(sizeof(FARMER) * nNorth);
	FARMER* fSouth = (FARMER*)malloc(sizeof(FARMER) * nSouth);

	/* Prepare Farmer Information */
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

	/* Create Threads */
	for (int i = 0; i < nNorth; ++i) {
		pthread_create(&(fNorth[i].tid), NULL, pass_bridge, &(fNorth[i]));
	}
	for (int i = 0; i < nSouth; ++i) {
		pthread_create(&(fSouth[i].tid), NULL, pass_bridge, &(fSouth[i]));
	}

	/* Wait for All Farmers to Pass the Bridge */
	for (int i = 0; i < nNorth; ++i) {
		pthread_join(fNorth[i].tid, NULL);
	}
	for (int i = 0; i < nSouth; ++i) {
		pthread_join(fSouth[i].tid, NULL);
	}

	/* Delete Farmer Arrays */
	free(fNorth);
	free(fSouth);

	/* Delete Condition Variable */
	pthread_cond_destroy(&c_condv);

} // end of main
