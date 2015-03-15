/*
Chad H. Glaser
KEYGEN:
	this file creates a key of of specified length
	the characters will be 27 allowed characters
	26 capital letters and 1 space
	I added 65 to the random number for ASCII Decimal values
	This program takes one argument, the length of the key


*/

#include<stdio.h>
#include<time.h>
#include<stdlib.h>

int main(int argc, char *argv[]){

	int keys = atoi(argv[1]);
	keys = keys - 1;
	int k;
	
	int i = 0;
	/*rand usage: http://www.cplusplus.com/reference/cstdlib/srand/*/
	srand(time(NULL));

	for(i; i < keys; i++){ 
		 
		k = (rand()%27);
		if (k == 26)
			printf("%c", 32);
		else
			printf("%c", (k+65));
	}
	printf("\n");
return 0;
}