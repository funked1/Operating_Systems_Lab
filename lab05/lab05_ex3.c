/** Daniel Funke
    CSC345-01
	Lab 5 Exercise 3 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define NUM_THREADS 5

/* Global variable definition */
int total_pts, pts_in_circle;
pthread_mutex_t lock;

/* function to be run in threads */
void* runner(void* param)
{
	int*  num_iterations = (int*) param;

	for (int i = 0; i < *num_iterations; ++i) {
		double x = (double)rand() / RAND_MAX;
		double y = (double)rand() / RAND_MAX;
		x = x * 2.0 - 1;
		y = y * 2.0 - 1;

		pthread_mutex_lock(&lock);
		if (sqrt(x * x + y * y) < 1.0){
			++pts_in_circle;
		}
		pthread_mutex_unlock(&lock);
	}

	pthread_exit(0);
}

int main(int argc, char** argv)
{	
	int pts_per_thread, remaining_pts;
	int pts[NUM_THREADS];
	
	total_pts = atoi(argv[1]);
	pthread_t tid[NUM_THREADS];
	pthread_t tid_r;

	/* Determine the number of iterations per thread */
	pts_per_thread = total_pts / NUM_THREADS;
	for (int i = 0; i < NUM_THREADS; ++i) {
		pts[i] = pts_per_thread;
	}

	/* Determine remainder iterations */
	remaining_pts = total_pts % NUM_THREADS;

	pthread_mutex_init(&lock, NULL);
	for(int i = 0; i < NUM_THREADS; ++i) {
		pthread_create(&tid[i], 0, runner, (void*) &pts[i]);
	}

	/* create an additional thread if total points is not
	   fully divisible by NUM_THREADS and pass remainder
	   to the new thread*/
	if (remaining_pts != 0) {
		pthread_create(&tid_r, 0, runner, (void*) &remaining_pts);
	}
	
	for(int i = 0; i < NUM_THREADS; ++i) {
		pthread_join(tid[i], NULL);
	}

	if (remaining_pts != 0) {
		pthread_join(tid_r, NULL);
	}

	/* estimate the value of pi using the monte carlo method */
	double pi_e = 4.0 * (double)pts_in_circle / (double)total_pts;

	printf("PI est. = %f\n", pi_e);

	pthread_mutex_destroy(&lock);

	return 0;
}
