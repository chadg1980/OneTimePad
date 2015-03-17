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
#include <sys/wait.h>
#include <signal.h>

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
	int len;
	
	memset(&outgone[0], 0, sizeof(outgone));
	
	snprintf(outgone, sizeof(msgOut), "%s", msgOut);
	len = strlen(outgone);
	
	bytesOut = send(nyFD,outgone, len+1, 0);
	//printf("Sent bytes %d\n, %s\n", bytesOut, outgone); 
	  
    if (bytesOut < 0){
		perror("ERROR writing to socket");
	}

	free(outgone);
}

/*-------------------------------------------RECV------------------------------*/
/*Steve Largent, seahawks reciver.Is my receive function*/
char *sLargent(int sockFD){
	
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
 
/*------------------------------------Big File Loop-----------------------*/
 
char *incomingText(int nyFD, char *returnText, int length ){
	
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
				sprintf(returnText, inBuf);
				//printf("%d's recv:\n%s",i, returnText);
				i += 1;
			}
			else {
				sprintf(returnText+strlen(returnText), inBuf);
				//printf("%d's recv:\n%s",i, returnText);
				i+=1;
				
			}
		}
		if(bytesTotal >= length)
				break;
		
	}
		
	free(inBuf);
	
	return returnText;
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
/*This function will create the cipher text and return the cipher text file*/
char *encode(char *text, char *key, char *cipher ){
	int i = (strlen(text)-1);
	int k = 0;
	int j;
	//printf("ENCODE: text len:%d\n", i);
	//printf("text\tkey\tcipher\n");
	for (k; k < i; k++){
		
		/*only dealing with the capital letters ASCII*/
		if( (text[k]>=65) && (text[k] <= 90) ){
			cipher[k] = ( (text[k]-65)   + (key[k]-65) );
			if(cipher[k] > 25)
				cipher[k] = cipher[k] % 25;
		}
		else if( text[k] == 32){
			cipher[k] = 91;
		}
		else{
			printf("This should have been caught already\n");
			
		}
		
		/*add 65 for ASCII printable letters*/
		if(cipher[k] != 91)
			cipher[k] = cipher[k] + 65;
	}
	cipher[i] = 10;
	
	//printf("ENCODE CIPHER:\n%s\n", cipher);
	return cipher;


}
/*This function will verify the sender as a decoder or send a kill signal*/
int verify(int nyFD){
	char *handshake = (char*) malloc(512);
	char kill[] = "99";
	char good[] = "2249";
	int secret;
	recv(nyFD, handshake, 512, 0 );
	secret = atoi(handshake);
	if(secret != 1138){
		outgoing(nyFD, kill);
		return 1;
	}
	else{
		outgoing(nyFD, good);
		return 3;
	}
}

/*This function makes sure the key is longer than the text, 
	and checks for no bad characters
	sends kill signal if there is an issue. */
int keyCheck(int nyFD, char* text, char *key){
	int lengthFirst = strlen(text);
	int lengthSecond = strlen(key);
	char toShort[] = "99";
	char badChar[] = "9999";
	char good[] = "2249";
	//printf("text: %s\n len%d\n", text, lengthFirst);
	//printf("key: %s\n len%d\n", key, lengthSecond);
	
	int i = (strlen(text)-1);
	int k = 0;
	int j;
	//printf("ENCODE: text len:%d\n", i);
	//printf("text\tkey\tcipher\n");
	for (k; k < i; k++){
		
		/*only dealing with the capital letters ASCII*/
		if( (text[k]>=65) && (text[k] <= 90) ){
			continue;
		}/*32 is the space*/
		else if( text[k] == 32){
			continue;
		}
		else{
			outgoing(nyFD, badChar);
			return 1;
		}
	}
	if(lengthFirst > lengthSecond){
		//printf("Key is not long enought\n");
		outgoing(nyFD, toShort);
		return 1;
	}
	else{
		outgoing(nyFD, good);
		return 3;
	}
}

sendCipher(int nyFD, char *cipher){
	char *goodToGo = (char *)malloc(1024);
	char *size = (char*) malloc(1024);
	int len = strlen(cipher);
	size_t sSent;
	int total = 0;
	memset(&goodToGo[0], 0, 1024);
	memset(&size[0], 0, 1024);
	
	/*receive G2G message*/
	goodToGo = sLargent(nyFD);
	
	/*send file size*/
	sprintf(size, "%d", len);
	outgoing(nyFD, size);
	
	/*receive G2G message*/
	goodToGo = sLargent(nyFD);
	
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
getTextSize(nyFD){
	char *textsizeFirst = (malloc(1024));
	int length;
	textsizeFirst = sLargent(nyFD);
	//printf("Size:%s\n", sizeFirst);
	
	length = atoi(textsizeFirst);
		
	free(textsizeFirst);
	return length;

}
getKeySize(nyFD){
	char *keysizeFirst = (malloc(1024));
	int length;
	keysizeFirst = sLargent(nyFD);
	//printf("Size:%s\n", sizeFirst);
	
	length = atoi(keysizeFirst);
	
	free(keysizeFirst);
	return length;

}
/*This function controls the sends and receivs*/
void switchBoard(int nyFD){
	
	char *plain;
	char *keyCode; 
	char *cipher;
	int plainSize;
	int keyCodeSize;
	
	int killed = 3;
	int i;
	killed = verify(nyFD);
	fflush(stdout);
	if (killed == 1){
		free(plain);
		free(keyCode);
		free(cipher);
		/*child process should exit, not the parent process*/
		exit(1);	
	
	}
	plainSize = getTextSize(nyFD);
	plain =  (char*) malloc(plainSize + 1024); 
	memset(&plain[0], 0, plainSize+1024);
	
	cipher =  (char*) malloc(plainSize + 1024); 
	memset(&cipher[0], 0, plainSize+1024);
	
	plain = incomingText(nyFD, plain, plainSize);
	
	keyCodeSize = getKeySize(nyFD);
	keyCode = (char*)malloc(keyCodeSize+1024);
	keyCode = incomingKey(nyFD, keyCode, keyCodeSize);
	
	
	killed = keyCheck(nyFD, plain, keyCode);
	fflush(stdout);
	if (killed == 1){
		free(plain);
		free(keyCode);
		free(cipher);
		/*child process should exit, not the parent process*/
		exit(1);	
	
	}
	
	cipher = encode(plain, keyCode, cipher);
	
	sendCipher(nyFD, cipher);
	
	free(plain);
	free(keyCode);
	free(cipher);
	close(nyFD);
	exit(0);
	

}


int main(int argc, char *argv[]){
	argCheck(argc);
	int sockfd, newSockFD, portNum;
	socklen_t cliLen;
    struct sockaddr_in serv_addr, cli_addr;
	int yes = 1;
	int notKilled = 1;
	pid_t anyPID = 0;
	pid_t spawnPID = -5;
	int status;
   
   /*Signal Area*/
   struct sigaction parent_act;
	parent_act.sa_handler = SIG_IGN;
	parent_act.sa_flags = 0;
	sigfillset(&(parent_act.sa_mask));
	sigaction(SIGINT, &parent_act, NULL);
	sigaction(SIGTSTP, &parent_act, NULL);
	/*END SIGNAL AREA*/
   
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
    while(notKilled == 1){
		
		anyPID = waitpid(-1, &status, WNOHANG) ;
		listen(sockfd, 10);
		 
		fflush(stdout);
		cliLen = sizeof(cli_addr);
		newSockFD = accept(sockfd, (struct sockaddr *) &cli_addr, &cliLen);
		if (newSockFD < 0) 
			  error("ERROR on accept");
		
		spawnPID = fork();
		
		if (spawnPID == 0){
			//printf("this is the child process\n");
			/*Child Signal Area*/
			struct sigaction child_act;
			child_act.sa_handler = SIG_DFL;
			child_act.sa_flags = 0;
			sigfillset(&(child_act.sa_mask));
			sigaction(SIGINT, &child_act, NULL);
			/*END SIGNAL AREA*/
			
			
			close(sockfd);
			switchBoard(newSockFD);
		}
		else if (spawnPID == -1){
			printf("Fork Failed\n");
			exit(1);
		}
		else{
			close(newSockFD);
		}
		
		
	}
    close(newSockFD);
    //close(sockfd);
    return 0; 
}

















