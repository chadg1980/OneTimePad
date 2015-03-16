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
	char *printMe = (char*)malloc(keys);
	int k;
	int i = 0;
	
	/*rand usage: http://www.cplusplus.com/reference/cstdlib/srand/*/
	srand(time(NULL));

	for(i; i < keys; i++){ 
		 
		k = (rand()%27);
		if (k == 26)
			printMe[i] = k;
		else
			printMe[i] = (k+65);
			//printf("%c", (k+65));
	}
	/*Adding the newline at the end makes life down the road easier
		I will do all my while loops look for the newline character!
	*/
	printMe[i] = 10;
	fprintf(stdout, "%s", printMe);
return 0;
}