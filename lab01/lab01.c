#include <stdio.h>
#include <stdlib.h>

int custom(int);

int main(int argc, char** argv)
{
	int user_input;
	
	user_input = atoi(argv[1]);
	printf("%d to the 5th power is %d\n", user_input, custom(user_input));

	return 0;
}
