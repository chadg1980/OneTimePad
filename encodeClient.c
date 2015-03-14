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


void argCheck(int argc, char* argv[]){
	
	if (argc < 3){
		fprintf(stderr," usage %s [plaintext][key][portNumber]\n", argv[0]);
		exit(2);
	}
}


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
		
	len = strlen(buffer);
		
	outBytes = send(sockFD, buffer, len+1, 0);
	
	if(outBytes < 0){
		perror("Error writing to socker");
	}
	/*printf("buffer is %s, I sent %d Bytes\n", buffer, outBytes); */
}
	/*-------------------------------------------RECV------------------------------*/
void incoming(sockFD){
	
	char *printBuf = (char*) malloc(512);
	char *recvBuf = (char*) malloc(512);
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
	free(printBuf);
	free(recvBuf);

}

void doStuff(int sockFD){
	
	outgoing(sockFD);
	incoming(sockFD);
	
}
/*-------------------------------------OPEN A TEXT FILE----------------------------*/
/*http://www.cplusplus.com/reference/cstdio/fread/*/
/*getting help with my loop: http://stackoverflow.com/questions/5594042/c-send-file-to-socket*/
openTextFile(int sockFD, char* myFile){
	FILE *textFile;
	long lSize;
	char *contents;
	char *sizeMatters;
	int total = 0;
	int sizeSent = 0;
	size_t bytesRead;
	
	textFile = fopen(myFile, "rb");
	
	/*get the file size*/
	fseek(textFile, 0, SEEK_END);
	lSize = ftell(textFile);
	rewind(textFile);
	
	/*allocate memory*/
	contents = (char*) malloc (sizeof(char)*lSize);
	if (contents == NULL){
		perror("Memory Error Reading text file:");
	}
	
	sprintf(sizeMatters, "%d", lSize);
	/*send size first*/
	send(sockFD, sizeMatters, 512, 0);
	incoming(sockFD);
	//printf("lSize: %d\n", lSize);
	/*Copy file into contents*/
	
	while ( (bytesRead = fread(contents, 1, 512, textFile)) > 0) {
			sizeSent = send(sockFD, contents, strlen(contents)+1, 0);
			//printf("%s", contents);
			total += sizeSent;
			//printf("total:%d\n bytesRead:%d\n", total, bytesRead);
			if (total >= lSize){
				break;
			}
			
			
	}
	incoming(sockFD);
	fclose(textFile);
	free(contents);
}

int main(int argc, char*argv[]){
	argCheck(argc, argv);
	int sockFD, portNum;
	int rv;
	int sizeKey, sizeText;
	struct sockaddr_in serv_addr;
    struct hostent *server;
	
	/*Take the CL arguments and get their size to make sure the key is larger than the plain text size*/
	sizeText = sizeof(argv[1]);
	sizeKey = sizeof(argv[2]);
	char* plainText = (char*) malloc(sizeText); ;
	char* key = (char*) malloc(sizeKey);
	
	
	plainText = argv[1];
	key = argv[2];
	portNum = atoi(argv[3]);
	sockFD = socket(AF_INET, SOCK_STREAM, 0);
	
	
	printf("sizeKey:%d sizeText:%d\n", sizeKey, sizeText);
	if (sizeText > sizeKey){
		printf("key %s too small\n", argv[2]);
		exit(1);
	}
	
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
	
	openTextFile(sockFD, plainText);
	openTextFile(sockFD, key);
	close(sockFD);
	return 0;
	
	
	
//return 0;
}























