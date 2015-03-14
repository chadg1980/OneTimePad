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
         fprintf(stderr,"ERROR, usage filename portnumber\n");
         exit(1);
     }

}
/*------------------------------SEND-------------------------------------*/
void outgoing(int nyFD, char *msgOut){
	char *outgone = (char*) malloc(500);
	
	int bytesOut = 0;
	
	memset(&outgone[0], 0, sizeof(outgone));
	
	snprintf(outgone, strlen(msgOut), "%s", msgOut);
	bytesOut = send(nyFD,outgone,strlen(outgone), 0);
	printf("Sent bytes %d\n", bytesOut); 
	  
    if (bytesOut < 0){
		error("ERROR writing to socket");
	}

	free(outgone);
}
 /*------------------------------------RCVD------------------*/
//char *incoming(int nyFD, char *gotten){
char *incoming(int nyFD){
	
	char *inBuf =  (char*) malloc(512);
	char *gotten =  (char*) malloc(512);
	//char inBuf[512];
	int bytesIN = 0;
	int bytesTotal = 0;
	int length;
		
	memset(&gotten[0], 0, 512);
	
	if (bytesIN = recv(nyFD, gotten, 512, 0 ) < 0){
		printf("nothing was received\n");
		sleep(1);
	}
	else{
		bytesTotal += bytesIN;
			printf("%s", gotten);
			printf("\n");
			
	}
    /*
	printf("I got %d bytes\n", bytesIN);
	length = strlen(gotten);
	printf("gotten is: %s", gotten);
	printf(" length of %d\n", length);
	*/
	/*
	 if (bytesTotal > 0){
		
		 snprintf(inBuf, bytesIN+1, "%s", gotten);
		 printf("Here is the message:\n ");
		 
		 printf("%s", inBuf);
		 printf("\n");
		 fflush(stdout);
	 
	 }
	else{
	 perror("ERROR reading from socket");
     }
	 */
	 
	 return gotten;
	 
}


void doStuff(nyFD){
	//char *gotten = (char*) malloc(500);
	//char gotten[512];
	char *back = (char*) malloc(500);
	int length;
	
		
		//back = incoming(nyFD, gotten);
		back = incoming(nyFD);
		
		
		outgoing(nyFD, back);
	
	

}
int main(int argc, char *argv[])
{
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
	
     doStuff(newSockFD);
     close(newSockFD);
     close(sockfd);
     return 0; 
}

















