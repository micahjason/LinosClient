/*
 * linos_server.c
 *
 *  Created on: Apr 8, 2019
 *      Author: root
 */


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>


int main(int argc, char *argv[]) {
	int sockfd, newsockfd, portno = 51717, clilen,err,ret_val;
	pthread_t tid;
	pid_t child_pid;
	struct sockaddr_in serv_addr, cli_addr;
	char data[10];
	//void *status;
	printf( "using port #%d\n", portno );

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0){
		perror("ERROR opening socket");
		return 1;
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons( portno );
	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
		perror("ERROR on binding");
		return 1;
	}
	listen(sockfd,20);
	clilen = sizeof(cli_addr);

	err = pthread_create(&(tid), NULL, &threader,NULL);
	//--- infinite wait on a connection ---
	//######################################################

	//#######################################################

	while ( 1 ) {
		printf( "waiting for new client...\n" );
		if ((newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, (socklen_t*) &clilen) ) < 0 ){
			perror("ERROR on accept");
			return 1;
		}
		printf( "opened new communication with client\n" );

		getData( newsockfd );

		printf( "ret_val1 : %d\n",ret_val);
		//--- send new data back ---
		if(ret_val==1){
			bzero(data,10);
			strcpy(data,"NOK");
			sendData( newsockfd,data);
		}
		else if(ret_val ==2){
			strcpy(data,"INOK");
			sendData( newsockfd,data);
		}
		else{
			bzero(data,10);
			strcpy(data,"OK");
			sendData( newsockfd,data);
		}
		close( newsockfd );

	}
	return ret_val;
}
