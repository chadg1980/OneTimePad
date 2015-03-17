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
	char *outgone = (char*) malloc(1024);
	
	int bytesOut = 0;
	int len = 0;
	memset(&outgone[0], 0, sizeof(outgone));
	
	snprintf(outgone, strlen(msgOut), "%s", msgOut);
	len = strlen(outgone);
	
	bytesOut = send(nyFD,outgone, len+1, 0);
	//printf("Sent bytes %d\n", bytesOut); 
	  
    if (bytesOut < 0){
		perror("ERROR writing to socket");
	}

	free(outgone);
}

 /*------------------------------------RCVD------------------*/
 char *receiver(int sockFD){
	char *recvBuf = (char*) malloc(1024);
	int  inBytes;
	memset(&recvBuf[0], 0, 1024);
		
	inBytes = recv(sockFD, recvBuf, 1024, 0);
	if (inBytes > 0){
		
		//printf("%s", recvBuf);
		//printf("Bytes: %d\n", inBytes);
		//printf("\n");
	}
	else{
		perror("ERROR reading from this socket");
	}
	//printf("GOOD RECEIVE\n");
	return(recvBuf);
 
 
 
 }
 
 /*------------------------------------------Incoming Cipher File---------------------------*/
char *incomingCipher(int nyFD, char returnCipher, int length){
	
	/*
	inBuf will take chunks, and copy that to returnText
	returnText will  for a size to allocate memory
	sizeFirst will take the size sent by the client, 
	which helps to know when to stop receiving data
	*/
	char *inBuf =  (char*) malloc(1024);
	//char *sizeFirst = (char*) malloc(1024);
	char goodCopy[] = "1111";
	memset(&inBuf[0], 0, 1024);
	//memset(&sizeFirst[0], 0, 1024);
	/*
	bytesIN couts the bytes from receive, 
	bytesTotal keeps a running total
	length is the int of the char* sizeFirst
	i is a flag to keep track if it is the first pass through or not for copying data
	*/
	size_t bytesIN;
	int bytesTotal = 0;
	//int length=0;
	int i = 0;
		
	
	/*----------------------------------------------------SEDNING REPLY--------------------------*/
	outgoing(nyFD, goodCopy);
	
		
	/*-------------------------------------------RECEIVE LOOP--------------------------------*/
	while (bytesTotal <= length){
		if (bytesIN = recv(nyFD, inBuf, 1024, 0 ) < 0){
			printf("nothing was received\n");
			sleep(1);
		}
		else{
			bytesIN = strlen(inBuf);
			bytesTotal += bytesIN;
			//printf("%s",inBuf);
			//printf("\nbytesIN:%d\n", bytesIN);
			//printf("pass: %d\n total: %d/Size%d\n",i,  bytesTotal, length);
			if (i == 0){
				sprintf(returnCipher, inBuf);
				//printf("%d's recv:\n%s",i, returnText);
				i += 1;
			}
			else {
				sprintf(returnCipher+strlen(returnCipher), inBuf);
				//printf("%d's recv:\n%s",i, returnText);
				i+=1;
				
			}
		}
		if(bytesTotal >= length)
				break;
		
	}
		
	free(inBuf);
	return returnCipher;
	 
}

char *incomingKey(int nyFD, char *returnKey, int length ){
	
	/*
	inBuf will take chunks, and copy that to returnKey
	returnKey will  for a size to allocate memory
	sizeFirst will take the size sent by the client, which helps to know when to stop receiving data
	*/
	char *inBuf =  (char*) malloc(1024);
	
	//char *sizeFirst = (char*) malloc(512);
	char goodCopy[] = "1111";
	/*
	bytesIN couts the bytes from receive, 
	bytesTotal keeps a running total
	length is the int of the char* sizeFirst
	i is a flag to keep track if it is the first pass through or not for copying data
	*/
	size_t bytesIN;
	int bytesTotal = 0;
	//int length;
	int i = 0;
		
	/*----------------------------------------------------SEDNING REPLY--------------------------*/
	outgoing(nyFD, goodCopy);
	
			
	/*-------------------------------------------RECEIVE LOOP--------------------------------*/
	while (bytesTotal <= length){
		if (bytesIN = recv(nyFD, inBuf, (1024), 0 ) < 0){
			printf("nothing was received\n");
			sleep(1);
		}
		else{
			bytesIN = strlen(inBuf);
			bytesTotal += bytesIN;
			//printf("%s",inBuf);
			//printf("\nbytesIN:%d\n", bytesIN);
			//printf("pass: %d\n total: %d/Size%d\n",i,  bytesTotal, length);
			if (i == 0){
				sprintf(returnKey, inBuf);
				//printf("%d's recv:\n%s",i, returnKey);
				
				i += 1;
			}
			else {
				sprintf(returnKey+strlen(returnKey), inBuf);
				//printf("%d's recv:\n%s",i, returnKey);
				i+=1;
				
			}
		}
		if(bytesTotal >= length)
				break;
		
	}
		
	free(inBuf);
	return returnKey;
}

/*This function will take the cipher text and key and return plain text*/
/*
void decode(char *cipherText, char* key){
	int i;
	int j = 0;
	char *plainText = (char *) malloc(1024*1024);
	printf("cipherText:\n%s", cipherText);
	while(cipherText[j] != 10){
		/*remove the 65 we added to make it printable*/
		/*if(cipherText[j] != 91){
			cipherText[j] = cipherText[j] - 65;
			plainText[j] = (cipherText[j] - ( key[j] - 65) ) ;
		
			if (plainText[j] < 0){
				plainText[j] = plainText[j] + 25;
				
			}
			
			if(plainText[j] >= 25){
				plainText[j] = plainText[j] % 25;
			}
			plainText[j] = plainText[j] + 65;
		}
		else
			plainText[j] = 32;
		j = j + 1;
	}
	plainText[j] = 10;
	printf("%s", plainText);
}
*/
/*This function will verify the sender as a decoder or send a kill signal*/
int verify(int nyFD){
	char *handshake = (char*) malloc(512);
	char kill[] = "99";
	char good[] = "31415";
	int secret;
	recv(nyFD, handshake, 512, 0 );
	secret = atoi(handshake);
	if(secret != 333){
		outgoing(nyFD, kill);
		return 1;
	}
	else{
		outgoing(nyFD, good);
		return 3;
	}
}
sendPlain(int nyFD, char *plain){
	char *goodToGo = (char *)malloc(1024);
	char *size = (char*) malloc(1024);
	int len = strlen(plain);
	size_t sSent;
	int total = 0;
	memset(&goodToGo[0], 0, 1024);
	memset(&size[0], 0, 1024);
	
	/*receive G2G message*/
	goodToGo = receiver(nyFD);
	
	/*send file size*/
	sprintf(size, "%d", len);
	outgoing(nyFD, size);
	
	/*receive G2G message*/
	goodToGo = receiver(nyFD);
	
	/*send loop*/
	while (1) {
		sSent = send(nyFD, cipher, 1024, 0);
		//printf("GOOD SEND\n");
		total += sSent;
		//printf("total:%d\n sent:%d\n", total, sSent);
		if (total >= len){
			break;
		}
	}
	free(goodToGo);
	free(size);
}	

/*--------------------------------------------------get the RECEIVE SIZE-----------------------*/
getCipherSize(nyFD){
	char *cipherSizeFirst = (malloc(1024));
	int length;
	textsizeFirst = receiver(nyFD);
	//printf("Size:%s\n", sizeFirst);
	
	length = atoi(cipherSizeFirst);
		
	free(cipherSizeFirst);
	return length;

}
getKeySize(nyFD){
	char *keysizeFirst = (malloc(1024));
	int length;
	keysizeFirst = receiver(nyFD);
	//printf("Size:%s\n", sizeFirst);
	
	length = atoi(keysizeFirst);
	
	free(keysizeFirst);
	return length;

}


void doStuff(nyFD){
	
	char *cipher;
	char *keyCode;
	char *plain;
	
	int cipherSize;
	int keyCodeSize;
	
	int killed = 3;
		
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

















