/*
	Chad H Glaser Cleint program
	This is based on an example found on the class material page
	I did not annotate each line that was used, but the TCP socket 
	connection was from that example. 


*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 


void error(const char *msg)
{
    perror(msg);
    exit(0);
}


	/*-------------------------------------SEND-------------------------*/
void outgoing(sockFD){
	char *buffer = (char*) malloc(512);
	char *sendBuf = (char*) malloc(512);
	int len,  outBytes;
	memset(&sendBuf[0], 0, sizeof(sendBuf));
	memset(&buffer[0], 0, sizeof(buffer));
	
	printf("Enter a message\n:");
	//fgets(buffer, 500, stdin);
	scanf("%[^\n]%*c", buffer);
	len = strlen(buffer);
	
	
	
	//snprintf(sendBuf, len, "%S", buffer);
	//len = strlen(sendBuf);
	outBytes = send(sockFD, buffer, len+1, 0);
	
	if(outBytes < 0){
		perror("Error writing to socker");
	}
	//printf("buffer is %s, I sent %d Bytes\n", buffer, outBytes); 
}
	/*-------------------------------------------RECV------------------------------*/
void incoming(sockFD){
	
	char *printBuf = (char*) malloc(500);
	char *recvBuf = (char*) malloc(500);
	int  inBytes, len;
	memset(&recvBuf[0], 0, sizeof(recvBuf));
	memset(&printBuf[0], 0, sizeof(printBuf));
	
	inBytes = recv(sockFD, recvBuf, 500, 0);
	if (inBytes > 0){
		snprintf(printBuf, inBytes+1, "%s", recvBuf);
		printf("%s", printBuf);
		printf("\n");
	}
	else{
		perror("ERROR reading from socket");
	}

}

void doStuff(int sockFD){
	
	outgoing(sockFD);
	incoming(sockFD);
	
}

int main(int argc, char*argv[]){

	int sockFD, portNum;
	int rv;
	int af;
	struct sockaddr_in serv_addr;
    struct hostent *server;
	
	
	if (argc < 2){
		fprintf(stderr," usage %s [portNumber]\n", argv[0]);
		exit(1);
	}
	
	portNum = atoi(argv[1]);
	sockFD = socket(AF_INET, SOCK_STREAM, 0);
	
	
	server = gethostbyname("localhost");
	
	if (server == NULL){
		fprintf(stderr, "ERROR, in host\n");
		exit(1);
	}
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char*)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portNum);
	if (connect(sockFD,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		perror("ERROR connecting");
	}
	doStuff(sockFD);
	
	close(sockFD);
	return 0;
	
	


return 0;
}























