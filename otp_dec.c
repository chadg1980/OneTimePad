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
#include <sys/wait.h>

void argCheck(int argc, char* argv[]){
	
	if (argc <= 3){
		fprintf(stderr," usage %s [ciphertext][key][portNumber]\n", argv[0]);
		exit(2);
	}
}


void error(const char *msg)
{
    perror(msg);
    exit(0);
}

/*-------------------------------------SEND-------------------------*/
void outgoing(int sockFD, char* message, int size ){
	
	char *sendBuf = (char*) malloc(1024);
	int outBytes = 0;
	int len = 0;
	memset(&sendBuf[0], 0, sizeof(sendBuf));
	snprintf(sendBuf, sizeof(sendBuf), "%s", message);
	outBytes = send(sockFD, sendBuf, sizeof(sendBuf), 0);
	//printf("Sent: %s\n", sendBuf);
	if(outBytes < 0){
		perror("Error writing to socker");
	}
	
}
	/*-------------------------------------------RECV------------------------------*/
char *incoming(int sockFD){
	
	
	char *recvBuf = (char*) malloc(1024);
	int  inBytes;
	memset(&recvBuf[0], 0, (1024));
	
	inBytes = recv(sockFD, recvBuf, (1024), 0);
	//printf("rec: %s\n", recvBuf);
	if (inBytes >= 0){
		//printf("REC: %s", recvBuf);
		//printf("Bytes: %d\n", inBytes);
		
	}
	else{
		perror("ERROR reading from this socket");
	}
	return(recvBuf);
}


void helloMSG (int sockFD){
	char secretHandshake[] = "333";
	int size = strlen(secretHandshake);
	char *returnVal = (char*)malloc(1024);
	int handShake;
	
	outgoing(sockFD, secretHandshake, size );
	returnVal = incoming(sockFD);
	handShake = atoi(returnVal);
		
	if (handShake == 99){
		printf("cannot find otp_dec_d on specified port\n");
		close(sockFD);
		exit(2);
	}
	free(returnVal);
}


void getPlain(int sockFD){

	/*
	inBuf will take 512 byte chunks, and copy that to gotten
	gotten will way for a size to allocate memory
	sizeFirst will take the size sent by the client, which helps to know when to stop receiving data
	*/
	
	char *gotten;
	char *sizeFirst = (char*) malloc(1024);
	char goodCopy[] = "1111";
	
	/*
	bytesIN couts the bytes from receive, 
	bytesTotal keeps a running total
	length is the int of the char* sizeFirst
	i is a flag to keep track if it is the first pass through or not for copying data
	*/
	size_t bytesIN;
	int bytesTotal = 0;
	int length;
	int i = 0;
	/*send ready to receive message*/
	outgoing(sockFD, goodCopy, 5);
	
	/*-----------------------------------------------------RECEIVE SIZE------------------------*/
	sizeFirst = incoming(sockFD);
	length = atoi(sizeFirst);
	gotten = (char*) malloc(length+1024);
	
	/*reset memory based on size*/
	memset(&gotten[0], 0, length+1024);
	
	/*----------------------------------------------------SEDNING REPLY--------------------------*/
	outgoing(sockFD, goodCopy, 5);
	
		
	/*----------------------------------------------------RECEIVE LOOP----------------------------*/
	while (bytesTotal <= length){
		bytesIN = recv(sockFD, gotten+bytesTotal, 1024, 0 );
			bytesTotal+= bytesIN;
		if(bytesTotal >= length)
				break;
	}
	
	printf("%s", gotten);
	
	free(sizeFirst);
	free(gotten);
	
}	

/*switch between the funtion calls*/
void doStuff(int sockFD, char *cipher, char*key){
	
	
	helloMSG(sockFD);
	
	openTextFile(sockFD, cipher);
	
	openTextFile(sockFD, key);
	
		
	getPlain(sockFD);
	
	
	
	
}
/*-------------------------------------OPEN A TEXT FILE----------------------------*/
/*http://www.cplusplus.com/reference/cstdio/fread/*/
/*getting help with my loop: http://stackoverflow.com/questions/5594042/c-send-file-to-socket*/
openTextFile(int sockFD, char* myFile){
	FILE *textFile;
	int lSize;
	char *contents;
	char *sizeMatters =(char*) malloc(1024);
	int total = 0;
	int sizeSent = 0;
	size_t bytesRead;
	char * goodToGo = (char*) malloc(1024);
	int gtg;
	memset(&sizeMatters[0], 0, 1024);
	memset(&goodToGo[0], 0, 1024);
	
	textFile = fopen(myFile, "rb");
	
	/*get the file size*/
	fseek(textFile, 0, SEEK_END);
	lSize = ftell(textFile);
	rewind(textFile);
	
	/*allocate memory*/
	contents = (char*) malloc(sizeof(char)*lSize);
	memset(&contents[0], 0, sizeof(char)*lSize);
	if (contents == NULL){
		perror("Memory Error Reading text file:");
	}
	
	/*send size first*/
	sprintf(sizeMatters, "%d", lSize);
	outgoing(sockFD, sizeMatters, 512);
	
	goodToGo = incoming(sockFD);
	gtg = atoi(goodToGo);
	if (gtg != 1111){
		printf("something went wrong\n");
		exit(1);
	}
	
	/*Copy file into contents*/
	while ( (bytesRead = fread(contents, 1, sizeof(char)*lSize, textFile)) > 0) {
		sizeSent = send(sockFD, contents, strlen(contents)+1, 0);
		//printf("GOOD SEND\n");
		//printf("fread loop: %ssent:%d\n", contents, sizeSent);
		//printf("%s", c);
		//printf("sent:%d", sizeSent);
		total += sizeSent;
		//printf("total:%d/ lSize:%d\n", total, lSize);
		//if (total >= lSize){
		//	break;
		//}
		
		
	}
	
	
	fclose(textFile);
	free(contents);
}
/*-----------------MAIN STARTS HERE----------------*/
int main(int argc, char*argv[]){
	argCheck(argc, argv);
	int sockFD, portNum;
	int rv;
	struct sockaddr_in serv_addr;
    struct hostent *server;
	
	char* cipherText = (char*) malloc(512); ;
	char* key = (char*) malloc(512);
		
	cipherText = argv[1];
	key = argv[2];
	portNum = atoi(argv[3]);
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
	
	
	doStuff(sockFD, cipherText, key);
		
	close(sockFD);
	return 0;

}























