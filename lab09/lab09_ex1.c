/** Dan Funke
	CSC345-01
	Lab 9 Exercise 1 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int total_frames;
int* FRAME;

int total_refs;
int* page_refs;

int pf_fifo, pf_lru, pf_opt;

void frame_init()
{	
	for (int i = 0; i < total_frames; ++i) {
		FRAME[i] = -1;
	}

	return;
}

void pr_fifo()
{
	pf_fifo = 0;	
	int idx;
	int fptr = 0;

	/* Initialize FRAME contents to -1 */
	frame_init();

	// implement FIFO to count pf_fifo
	for (int i = 0; i < total_refs; ++i) {
		idx = -1;
		for (int j = 0; j < total_frames; ++j) {
			/* no page fault; continue to next page request */
			if (page_refs[i] == FRAME[j]) {
				idx = j;
				break;
			}
		}

		/* page fault; insert page request into appropriate slot */
		if (idx == -1) {	
			++pf_fifo;
			FRAME[fptr] = page_refs[i];
			fptr = (fptr + 1) % total_frames; 
		}
	}
		
	return;
}

void pr_lru()
{
	pf_lru = 0;
	int idx;

	/* Initialize FRAME contents to -1 */
	frame_init();

	// implement LRU to count pf_lru
	for (int i = 0; i < total_refs; ++i){
		idx = -1;
		for (int j = 0; j < total_frames; ++j) {	
			if (FRAME[j] == page_refs[i]) {
				idx = j;
				break;
			}
		}	

		/* Page fault; add page request to the front, remove back page */
		if (idx == -1 || idx == (total_frames - 1)) {
			++pf_lru;
			/* shift FRAME contents back 1 place */
			for (int j = (total_frames - 1); j > 0; --j) {
				FRAME[j] = FRAME [j - 1];
			}
			/* add new request to the front */
			FRAME[0] = page_refs[i];
		}
		/* No Page Fault; move page index to top, shift frame contents back */
		else {
			for (int j = idx; j > 0; --j) {
				FRAME[j] = FRAME[j - 1];
			}
			FRAME[0] = page_refs[i];
		}

	}
	
	return;
}


void pr_optimal()
{
	pf_opt = 0;
	int idx, ref;
	int* ref_idx;

	// Initialize FRAME contents to -1
	frame_init();

	// Create table to hold temporal reference information
	// and initialize all values to -1
	ref_idx = (int*)malloc(sizeof(int) * total_frames);
	for (int i = 0; i < total_frames; ++i) {
		ref_idx[i] = -1;
	}

	// Implement Optimal PR algorithm to count pf_opt
	for (int i = 0; i < total_refs; ++i) {
		idx = -1; // assume page fault

		for (int j = 0; j < total_frames; ++j) {
			if (page_refs[i] == FRAME[j]) {
				idx = j;
			}
		}

		if (idx == -1) {
			++pf_opt;

			// Determine if FRAME has any empty entries	
			int empty_idx = -1; // assume FRAME is full
			for (int k = 0; k < total_frames; ++k) {
				if (FRAME[k] == -1) {	
					empty_idx = k;
					break;
				}	
			}

			// Fill empty FRAME entry
			if (empty_idx >= 0) {
				FRAME[empty_idx] = page_refs[i];
			}
			// Replace unreferenced/later referenced entry
			else {
				// Determine if FRAME entries are referenced again
				// if they are referenced, store index in ref_idx
				for (int k = 0; k < total_frames; ++k) {
					ref = -1; // assume entry is not referenced again
					for (int l = i; l < total_refs; ++l) {
						if (FRAME[k] == page_refs[l]) {
							ref = l;
							break;
						}
					}
					ref_idx[k] = ref;
				}

				// Determine which entry to replace
				int nr_idx = -1; // assume all entries are referenced again
				for (int k = 0; k < total_frames; ++k) {
					if (ref_idx[k] == -1) {
						nr_idx = k;
						break;
					}
				}

				// Replace first unreferenced entry
				if (nr_idx >= 0) {
					FRAME[nr_idx] = page_refs[i];
				}
				// Replace entry referenced last
				else {	
					int max_idx = 0;
					for (int k = 1; k < total_frames; ++k) {
						if (ref_idx[k] > ref_idx[max_idx]) {
							max_idx = k;
						}
					}	
					FRAME[max_idx] = page_refs[i];
				}
			}

		}	
		
	} // end main for loop

	// Free allocated memory
	free(ref_idx);

} // end pr_optimal



int main (int argc, char** argv)
{
	total_frames = atoi(argv[1]);
	if (total_frames < 1 || total_frames > 7) {
		printf("Total frames must be in range [1, 7]\n");
		return -1;
	}

	/* Create FRAME array and initialize values to -1 */
	FRAME = (int*)malloc(sizeof(int) * total_frames);


	FILE* fp = fopen("input.txt", "rt");
	if (fp == NULL) return -1;

	fscanf(fp, "%d", &total_refs);

	page_refs = (int*)malloc(sizeof(int) * total_refs);

	for (int i = 0; i < total_refs; ++i) {
		fscanf(fp, "%d", &(page_refs[i]));
	}

	fclose(fp);

	// Run FIFO
	pr_fifo();

	// Run LRU
	pr_lru();

	// Run optimal
	pr_optimal();

	// generate report
	fp = fopen("output.txt", "wt");

	fprintf(fp, "FIFO: %d\n", pf_fifo);
	fprintf(fp, "LRU: %d\n", pf_lru);
	fprintf(fp, "Optimal: %d\n", pf_opt);
	

	free(page_refs);
	free(FRAME);

	return 0;
}
