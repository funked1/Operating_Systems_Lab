#include <stdio.h>
#include <stdlib.h>

#define MAX_SEC 5000
#define NUM_REQ 1000

int main (int argc, char** argv)
{
	FILE* fp = fopen("input.txt", "wt");

	fprintf(fp, "%d\n", MAX_SEC);
	fprintf(fp, "%d\n", NUM_REQ);

	for (int i = 0; i < NUM_REQ; ++i) {
		fprintf(fp, "%d\n", rand() % MAX_SEC);		
	}

	fprintf(fp, "%d\n", rand() % MAX_SEC);

	fclose(fp);
	
	return 0;
}
