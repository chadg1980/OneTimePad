/* 
	Chad H glaser
	This program started as an example from supplied material.
	I did not annotate each line of code that was taken from this example, but 
	The TCP server is directly copy and pasted from the examples:
	Networking Demo: Server
		https://oregonstate.instructure.com/courses/1498922/pages/block-4

A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>





void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void argCheck(int argCount){
	if (argCount < 2) {
		fprintf(stderr,"ERROR, usage filename plaintext key portnumber\n");
		exit(1);
    }

}
/*------------------------------SEND-------------------------------------*/
void outgoing(int nyFD, char *msgOut){
	char *outgone = (char*) malloc(500);
	
	int bytesOut = 0;
	char tempMsg[12] = "testing 123";
	
	memset(&outgone[0], 0, sizeof(outgone));
	
	//snprintf(outgone, 13, "%s", msgOut);
	bytesOut = send(nyFD,tempMsg,13, 0);
	printf("Sent bytes %d\n", bytesOut); 
	  
    if (bytesOut < 0){
		error("ERROR writing to socket");
	}

	free(outgone);
}
 /*------------------------------------RCVD-----------------------*/

char *incoming(int nyFD){
	
	/*
	inBuf will take 512 byte chunks, and copy that to gotten
	gotten will way for a size to allocate memory
	sizeFirst will take the size sent by the client, which helps to know when to stop receiving data
	*/
	char *inBuf =  (char*) malloc(512);
	char *gotten;
	char *sizeFirst = (char*) malloc(512);
	
	/*
	bytesIN couts the bytes from receive, 
	bytesTotal keeps a running total
	length is the int of the char* sizeFirst
	i is a flag to keep track if it is the first pass through or not for copying data
	*/
	int bytesIN;
	int bytesTotal = 0;
	int length;
	int i = 0;
	size_t gottenSize;
	
	recv(nyFD, sizeFirst, 512, 0 );
	length = atoi(sizeFirst);
	gotten = (char*) malloc(length+512);
	
	memset(&gotten[0], 0, length+512);
	outgoing(nyFD, sizeFirst);
	while (bytesTotal <= length){
		if (bytesIN = recv(nyFD, inBuf, 512, 0 ) < 0){
			printf("nothing was received\n");
			sleep(1);
		}
		else{
			bytesIN = strlen(inBuf);
			bytesTotal += bytesIN;
			//printf("bytesIN:%d\n", bytesIN);
			//printf("%d/%d\n", bytesTotal, length);
			if (i == 0){
				sprintf(gotten, inBuf);
				i += 1;
			}
			else {
				sprintf(gotten+bytesIN, inBuf);
				
			}
		}
		if(bytesTotal >= length)
				break;
	}
	 //printf("%s", gotten);
	outgoing(nyFD, sizeFirst);
	 free(inBuf);
	 free(sizeFirst);
	 return gotten;
	 
}

/*This function will create the cipher text and return the cipher text file*/
void encode(char *text, char *key ){
	int i = (strlen(text)-1);
	int k = 0;
	int letter;
	printf("i: %d\n", i);
	for (k; k < i; k++){
		printf("%c",text[k]);
		int letter = text[k] - 65;
			if(letter < 0){
				letter += 65;
			}
		printf("     %d\n", letter);
	}



}

/*This function controls the sends and receivs*/
void switchBoard(int nyFD){
	
	char *backFirst = (char*) malloc(1024*1024);
	char *backSecond = (char*) malloc(1024*1024);
	int length;
	backFirst = incoming(nyFD);
	backSecond = incoming(nyFD);
	encode(backFirst, backSecond);
	
	//printf("backFirst:%s, backSecond:%s\n", backFirst, backSecond);
	
	free(backFirst);
	free(backSecond);
	
	

}


int main(int argc, char *argv[]){
	argCheck(argc);
	int sockfd, newSockFD, portNum;
	socklen_t cliLen;
    struct sockaddr_in serv_addr, cli_addr;
	int yes = 1;
   
   /*getting a socket. Sock_stream means TCP*/
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
     
	 if (sockfd < 0) 
        error("ERROR opening socket");
     
	
	memset(&serv_addr, 0, sizeof(serv_addr));
    portNum = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portNum);
     
	 if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
			  
	/*I was getting a lot of refused connection errors, 
	so I pulled this line of code from Beej's guide hoping it would fix it up.*/
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
     listen(sockfd,5);
     
	 fflush(stdout);
	 cliLen = sizeof(cli_addr);
     newSockFD = accept(sockfd, (struct sockaddr *) &cli_addr, &cliLen);
     if (newSockFD < 0) 
          error("ERROR on accept");
	
     switchBoard(newSockFD);
     close(newSockFD);
     close(sockfd);
     return 0; 
}

















