/** Daniel Funke
    CSC345-01
    Lab 3 Exercise 2 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int total_pts, pts_in_circle;

void* runner(void* param)
{
	for (int i = 0; i < total_pts; ++i) {
		double x = (double)rand() / RAND_MAX;
		double y = (double)rand() / RAND_MAX;
		x = x * 2.0 - 1;
		y = y * 2.0 - 1;

		if (sqrt(x * x + y * y) < 1.0){
			++pts_in_circle;
		}
	}

	pthread_exit(0);
}

int main(int argc, char** argv)
{
	pthread_t tid;
	total_pts = atoi(argv[1]);
	
	pthread_create(&tid, 0, runner, NULL);
	pthread_join(tid, NULL);

	double pi_e = 4.0 * (double)pts_in_circle / (double)total_pts;

	printf("PI est. = %f\n", pi_e);

	return 0;
}
