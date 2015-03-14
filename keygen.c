/*
Chad H. Glaser
KEYGEN:
	this file creates a key of of specified length
	the characters will be 27 allowe4d characters
	26 capital letters and 1 space
	
	This program takes one argument, the length of the key


*/

#include<stdio.h>
#include<time.h>
#include<stdlib.h>

int main(int argc, char *argv[]){

int x = atoi(argv[1]);
char *i = (malloc (x*sizeof(char)));
/*rand usage: http://www.cplusplus.com/reference/cstdlib/srand/*/
srand(time(NULL));


for(x; x > 0; x--){ 
	 
	printf("%d", rand()%27+1);
}
printf("\n");
return 0;
}