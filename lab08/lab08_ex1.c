/** Dan Funke
    CSC345-01
    Lab 8 Exercise 1 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PAGE_SIZE 4096  

int main (int argc, char** argv)
{
	long unsigned address;
	unsigned int page_num;
	unsigned int offset;

	address = atol(argv[1]);
	page_num = address / PAGE_SIZE;
	offset = address % PAGE_SIZE;

	printf("The address %lu contains:\n", address);
	printf("page number = %d\n", page_num);
	printf("offset = %d\n", offset);

}
