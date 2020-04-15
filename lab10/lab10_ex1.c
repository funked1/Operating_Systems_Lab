/** Dan Funke
    CSC345
    Lab 10 Exercise 1 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int max_sec;
int num_req;
int cur_loc;

int* req;

int fcfs()
{
	int total = 0;
	int cur = cur_loc;

	for (int i = 0; i < num_req; ++i) {
		total += abs(req[i] - cur);
		cur = req[i];
	}

	return total;
}

int scan()
{
	int total = 0;
	int cur = cur_loc;
	int * req_tmp = (int*)malloc(sizeof(int) * num_req);
	
	int i, j;

	for (i = 0; i < num_req; ++i) {
		req_tmp[i] = req[i];
	}
	
	// sort req in increasing order
	for (i = 0; i < num_req; ++i) {
		for (j = i; j < num_req; ++j) {
			if (req_tmp[j] < req_tmp[i]) {
				int t = req_tmp[j];
				req_tmp[j] = req_tmp[i];
				req_tmp[i] = t;
			}
		}
	}

	// decide direction
	for (i = 0; i < num_req; ++i) {
		if (cur < req_tmp[i])
		break;
	}

	if (i == 0) {
		// left to right (low to high)
		// compute distance in the direction using similar to fcfs
		for (j = 0; j < num_req; ++j) {
			total += abs(req_tmp[j] - cur);
			cur = req_tmp[j];
		}
	} else if (i == num_req) {
		// right to left (high to low)
		// compute distance in the direction using similar to fcfs
		for (j = (num_req - 1); j >= 0; --j) {
			total += abs(req_tmp[j] - cur);
			cur = req_tmp[j];
		}
	} else {
		if (abs(cur - req_tmp[i - 1]) < abs(cur - req_tmp[i])){
			// right to left (high to low)
			for (j = (i - 1); j >= 0; --j) {
				total += abs(req_tmp[j] - cur);
				cur = req_tmp[j];
			}
			cur = 0;

			// jump to original location and compute distance of remainder
			for (j = i; j < num_req; ++j) {
				total += abs(req_tmp[j] - cur);
				cur = req_tmp[j];
			}

		} else {
			// left to right (low to high)
			for (j = i; j < num_req; ++j) {
				total += abs(req_tmp[j] - cur);
				cur = req_tmp[j];
			}
			cur = max_sec;

			// jump to the original location and compute distance of remainder
			for (j = (i - 1); j >= 0; -- j) {
				total += abs(req_tmp[j] - cur);
				cur = req_tmp[j];
			}
		}
	}	

	free(req_tmp);

	return total;
}

int cscan()
{
	// sort req
	int total = 0;
	int cur = cur_loc;
	int * req_tmp = (int*)malloc(sizeof(int) * num_req);
	
	int i, j;

	// Create temporary copy of request array
	for (i = 0; i < num_req; ++i) {
		req_tmp[i] = req[i];
	}
	
	// sort req_tmp in increasing order
	for (i = 0; i < num_req; ++i) {
		for (j = i; j < num_req; ++j) {
			if (req_tmp[j] < req_tmp[i]) {
				int t = req_tmp[j];
				req_tmp[j] = req_tmp[i];
				req_tmp[i] = t;
			}
		}
	}

	// Determine index of current request
	for (i = 0; i < num_req; ++i) {
		if (cur < req_tmp[i])
		break;
	}

	// compute distance in increasing direction
	for (j = i; j < num_req; ++j) {
		total += abs(req_tmp[j] - cur);
		cur = req_tmp[j];
	}
	cur = max_sec;

	// jump to the beginning of the disk
	total += abs(0 - cur);
	cur = 0;

	// compute distance of remainder
	for (j = 0 ; j < i; ++j) {
		total += abs(req_tmp[j] - cur);
		cur = req_tmp[j];
	}

	return total;
}

int main (int argc, char** argv)
{
	FILE* fp = fopen("input.txt", "rt");
	
	fscanf(fp, "%d", &max_sec);
	fscanf(fp, "%d", &num_req);

	req = (int*)malloc(sizeof(int) * num_req);

	for (int i = 0; i < num_req; ++i) {
		fscanf(fp, "%d", &(req[i]));
	}

	fscanf(fp, "%d", &cur_loc);
	
	fclose(fp);

	/*
	for (int i = 0; i < num_req; ++i) {
		printf("%d\n", req[i]);
	}
	printf("%d, %d, %d\n", max_sec, num_req, cur_loc);
	*/

	fp = fopen("output.txt", "wt");
	
	fprintf(fp, "%d\n", fcfs());
	fprintf(fp, "%d\n", scan());
	fprintf(fp, "%d\n", cscan());



	fclose(fp);

	free(req);	

    return 0;
}
