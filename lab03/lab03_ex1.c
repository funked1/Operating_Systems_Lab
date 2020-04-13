/** Daniel Funke
    CSC345-01
    Lab 3 Exercise 1 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

/* Declaration of global variable */
int len;
int* nums;
int r_min, r_max;
float r_avg, r_med, r_sig;

/* Function to calculate minimum value from set of integers */
void *myMin (void *param)
{
	int i;
	r_min = nums[0];
	for(i = 0; i < len; ++i) {
		if (r_min > nums[i]) r_min = nums[i];
	}

	pthread_exit(0);
}

/* Function to calculate maximum value from set of integers */
void *myMax (void *param)
{
	int i;
	r_max = nums[0];
	for (i = 0; i < len; ++i){
		if (r_max < nums[i]) r_max = nums[i];
	}

	pthread_exit(0);
}

/* Function to calculate average value of set of integers */
void *myAvg (void *param)
{
	int i;
	r_avg = 0;
	for (i = 0; i < len; ++i){
		r_avg = r_avg + nums[i];
	}
	r_avg = r_avg / len;
	
	pthread_exit(0);
}

/* Function to calculate the median value of a set of integers */
void *myMed (void *param)
{
	// bubble sort array from min to max
	int i, j, temp;
	for (i = 0; i < len; ++i) {
		for (j = 0; j < len - i - 1; ++j) {
			if (nums[j] > nums[j + 1]) {
				temp = nums[j];
				nums[j] = nums[j + 1];
				nums[j + 1] = temp;
			}
		}
	}

	/* Determine median value by taking middle value if length
	   is odd, or averaging two middle values if length is even */
	if (len % 2 == 0) {
		r_med = (nums[len/2] + nums[(len/2) -1]) / 2;
	} else {
		r_med = nums[len/2];
	}

	pthread_exit(0);
}

/* Function to calculate the standard deviation of a set of integers */
void *mySig (void *param)
{
	// calculate average value
	int i;
	float mean = 0;
	for (i = 0; i < len; ++i) {
		mean = mean + nums[i];
	}
	mean = mean / len;

	/* calculate sum of difference between sample and mean squared */
	float diff, sum, square = 0;
	for (int i = 0; i < len; ++i) {
		diff = nums[i] - mean;
		square = diff * diff;
		sum = sum + square;
	}
	
	/* calculate standard deviation by taking square root of variance */
	r_sig = sqrt(sum / len);

	pthread_exit(0);
}


int main (int argc, char** argv)
{
	if (argc < 2) {
		printf("error\n");
		return 1;
	}
	len = argc - 1;
	nums = (int*)malloc(sizeof(int)*len);
	for (int i = 1; i < argc; ++i) {
		nums[i - 1] = atoi(argv[i]);
	}

	/* create child threads to perform statistical functions */
	pthread_t tid1, tid2, tid3, tid4, tid5;
	pthread_create(&tid1, 0, myMin, NULL);
	pthread_create(&tid2, 0, myMax, NULL);
	pthread_create(&tid3, 0, myAvg, NULL);
	pthread_create(&tid4, 0, myMed, NULL);
	pthread_create(&tid5, 0, mySig, NULL);

	/* join child threads with parent process */
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_join(tid3, NULL);
	pthread_join(tid4, NULL);
	pthread_join(tid5, NULL);

	/* output results */
	printf("Min = %d\n", r_min);
	printf("Max = %d\n", r_max);
	printf("Avg = %f\n", r_avg);
	printf("Med = %f\n", r_med);
	printf("Sig = %f\n", r_sig);
	
	
	free(nums);
	return 0;
}

