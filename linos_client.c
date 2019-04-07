/*
 ============================================================================
 Name        : linos_client.c
 Author      : Micah Kangogo
 Version     :
 Copyright   : Your copyright notice
 Description : Linos Client
 ============================================================================
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <arpa/inet.h>

void strip_extra_spaces(char* str);
void remove_new_line(char* string);

char * buff;

void error(char *msg) {
	perror(msg);
	exit(0);
}

void sendData( int sockfd, char x[32] ) {
	int n;

	char buffer[32];
	sprintf( buffer, "%s\n", x );
	if ( (n = write( sockfd, buffer, strlen(buffer) ) ) < 0 ){
		perror("ERROR writing to socket");
	}
	buffer[n] = '\0';
}

char * getData( int sockfd ) {
	char buffer[32];
	int n;

	bzero(buffer,32);
	if ( (n = read(sockfd,buffer,31) ) < 0 ){
		perror( "ERROR reading from socket" );
	}

	buff = (char*)malloc(32);
	bzero(buff,32);
	strcpy(buff,buffer);

	return buff;
}

int main(int argc, char *argv[])
{
	int sockfd,ret_val, portno = 51717;
	char serverIp[] = "10.42.0.202";
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char str[32]="";

	if (argc < 3) {
		// error( const_cast<char *>( "usage myClient2 hostname port\n" ) );
		printf( "contacting %s on port %d\n", serverIp, portno );
		// exit(0);
	}
	if ( ( sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 ){
		perror("ERROR opening socket");
		return 100;
	}

	if ( ( server = gethostbyname( serverIp ) ) == NULL ){
		perror("ERROR, no such host\n" );
		return 100;
	}

	printf( "argv[0] %s \n",argv[1]);

	bzero( (char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy( (char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if ( connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
		perror("ERROR connecting");
		return 100;
	}
	//char str[32] = "open|101|27";
	bzero(str,32);
	sprintf(str,"%s",argv[1]);
	sendData( sockfd, str );
	getData( sockfd);
	printf("buff %s\n",buff );
	usleep(1000);
	remove_new_line(buff);
	strip_extra_spaces(buff);

	if(strcmp(buff,"NOK")==0){
		ret_val = 101;
	}
	else if(strcmp(buff,"INOK")==0){
		ret_val = 102;
	}
	else{
		ret_val = 0;
	}
	close( sockfd );
	printf("ret_val : %d\n",ret_val );
	return ret_val;
}

void remove_new_line(char* string)
{
	int length = strlen(string);
	while(length>0)
	{
		if(string[length-1] == '\n')
		{
			--length;
			string[length] ='\0';
		}
		else
			break;
	}
}

void strip_extra_spaces(char* str) {
	int i,x;
	for(i=x=1; str[i]; ++i)
		if(!isspace(str[i])/* ||  (i>0 && !isspace(str[i-1]))*/)
			str[x++] = str[i];
	str[x] = '\0';
}
