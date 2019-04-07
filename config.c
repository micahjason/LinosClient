/*
 * config.c
 *
 *  Created on: Apr 8, 2019
 *      Author: root
 */


/*
 * curl.c
 *
 *  Created on: Apr 1, 2019
 *      Author: root
 */

#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <ctype.h>
#include "main.h"

int gpioWrite(int);
void remove_new_line(char* string);
void strip_extra_spaces(char* str);

int curl_client()
{
	//linosPtr searchhost = NULL;
	char req[1024];
	/*char req[1024];
	char uid[10]="12345";
	char api_key[100] = "0d1d07e612f892e0211d9738b56568d0c5a4b1723af18305366908d8bc58ef74";
	char method[10] = "GET";
	char protocol[] = "http://";
	char ip[20] = "192.168.0.101";
	char port[10] = "8080";
	char content_type[20] = "application/json";
	char endpoint[100] = "/CentralParkingInterface/emix/parking/event/500/device/control/";
	 */

	if (!linosserver){
		puts("No online servers to poll");
		return 1;
	}

	//for (searchhost=linosserver; searchhost != NULL; searchhost = (linosPtr)(searchhost->ah.next)) {
		bzero(req,1024);
		//timer = atoi(searchhost->watch);

		sprintf(req,"curl -X %s %s%s:%s%s -H 'api_key: %s' -H 'content-type: %s' -H 'uid: %s'",linosserver->api_method,linosserver->api_protocol,linosserver->server_ip,linosserver->server_port,linosserver->endpoint,linosserver->api_key,linosserver->api_content_type,linosserver->linos_uid);
		//strip_extra_spaces(req);
		remove_new_line(req);
		printf("req : %s\n",req);
		system(req);
	//}

	/*bzero(req,1024);

	sprintf(req,"curl -X %s %s%s:%s%s -H 'api_key: %s' -H 'content-type: %s' -H 'uid: %s'",linosserver->api_method,linosserver->api_protocol,linosserver->server_ip,linosserver->server_port,linosserver->endpoint,linosserver->api_key,linosserver->api_content_type,linosserver->linos_uid);
	//strip_extra_spaces(req);
	remove_new_line(req);
	printf("req : %s\n",req);
	system(req);*/

	return 0;
}

int LinosGPIO() {
	FILE *config_fp = NULL;
	char line[1300 + 1];
	char *command, *channel, *gpio,*code;
	linosgpioPtr tmp = NULL;
	printf("############################################\n\n");
	//open|101|27|127
	//close|101|17|217
	//sprintf(path,"linosgpio.cfg");
	config_fp = fopen("linosgpio.cfg", "r");

	if (config_fp == NULL) {
		perror("Error INFO: ");
		return 1;
	}
	else
	{
		while (fgets(line, 1300, config_fp) != NULL) {

			command = strtok(line, "|");
			if (command) {
				channel = strtok( NULL, "|");
				if (channel) {
					gpio = strtok( NULL, "|");

					if(gpio){
						code = strtok( NULL, "|");
					}
				}
			}
			printf("LinosGPOI FIND command : %s\nchannel : %s\ngpio : %s\ncode : %s\n",command, channel, gpio,code);
			tmp = (linosgpioPtr) malloc(sizeof(LinosIO));
			if (tmp == NULL) {
				perror("Error INFO : ");
				return 1;
			}
			strcpy(tmp->command,command);
			strcpy(tmp->channel, channel);
			strcpy(tmp->gpio, gpio);
			strcpy(tmp->code, code);

			printf("tmp->command :%s\n",tmp->command);
			printf("tmp->channel :%s\n",tmp->channel);
			printf("tmp->gpio :%s\n",tmp->gpio);
			printf("tmp->code :%s\n",tmp->code);

			HASH_ADD(hh, linosgpio, code, strlen(code),tmp);

		}
	}
	if (config_fp) {
		fclose(config_fp);
	}
	return 0;
}

int GPIO(char buff[]){

	linosgpioPtr tmp = NULL;
	char * cmd;
	char * code;
	int pin;
	char * gpio;

	cmd = (char*)malloc(10);
	code = (char*)malloc(10);
	strcpy(cmd,strtok(buff,"|"));
	strcpy(code,strtok(NULL,"|"));

	printf( "cmd : %s , code : %s\n", cmd,code);
	tmp = (linosgpioPtr) malloc(sizeof(LinosIO));

	HASH_FIND(hh,linosgpio,code, strlen(code), tmp);
	if(tmp!=NULL){
		puts("\n\nDuplicate Found\n\n");

		if(strcmp(tmp->command,cmd)==0){
			pin = atoi(tmp->gpio);
			gpioWrite(pin);
		}
		else{
			return 2;
		}
	}
	else{
		return 1;
	}
	return 0;
}

int LinosServer(){
	char *location,*linos_name,*linos_uid,*watch,*server_ip,*server_port,*endpoint,*api_key,*api_method,*api_protocol,*api_content_type;
	FILE *config_fp = NULL;
	char line[2048 + 1];
	linosPtr tmp = NULL;

	printf("############################################\n\n");
	config_fp = fopen("linosserver.cfg", "r");

	if (config_fp == NULL) {
		perror("Error INFO: ");
		return 1;
	}
	else
	{
		while (fgets(line,2048, config_fp) != NULL) {

			strip_extra_spaces(line);
			remove_new_line(line);
			location = strtok(line, "|");
			if (location) {
				linos_name = strtok( NULL, "|");
				if (linos_name) {
					linos_uid = strtok( NULL, "|");

					if(linos_uid){
						watch = strtok( NULL, "|");

						if(watch){
							server_ip= strtok( NULL, "|");

							if(server_ip){
								server_port = strtok( NULL, "|");

								if(server_port){
									endpoint = strtok( NULL, "|");

									if(endpoint){
										api_key = strtok( NULL, "|");

										if(api_key){
											api_method = strtok( NULL, "|");

											if(api_method){
												api_protocol = strtok( NULL, "|");

												if(api_protocol){
													api_content_type = strtok( NULL, "|");


												}
											}
										}
									}
								}
							}
						}
					}
				}
			}

			//printf("LinosGPOI FIND command : %s\nchannel : %s\ngpio : %s\ncode : %s\n",command, channel, gpio,code);
			tmp = (linosPtr)malloc(sizeof(Linos));
			if (tmp == NULL) {
				perror("Error INFO : ");
				return 1;
			}
			strcpy(tmp->location,location );
			strcpy(tmp->linos_name, linos_name);
			strcpy(tmp->linos_uid, linos_uid);
			strcpy(tmp->watch, watch);
			strcpy(tmp->server_ip, server_ip);
			strcpy(tmp->server_port, server_port);
			strcpy(tmp->endpoint, endpoint);
			strcpy(tmp->api_key, api_key);
			strcpy(tmp->api_method, api_method);
			strcpy(tmp->api_protocol, api_protocol);
			strcpy(tmp->api_content_type, api_content_type);

			printf("tmp->location :%s\n",tmp->location);
			printf("tmp->linos_name :%s\n",tmp->linos_name);
			printf("tmp->linos_uid :%s\n",tmp->linos_uid);
			printf("tmp->watch :%s\n",tmp->watch);

			HASH_ADD(ah, linosserver, server_ip, strlen(server_ip),tmp);
		}
	}

	if (config_fp) {
		fclose(config_fp);
	}
	return 0;
}

void* threader(void *arg)
{
	int ret_val;
	time_t start, current,t;
	start = time(NULL);
	printf("\n\n\nSeconds :%ld\n\n\n",start);
	do {

		current = time(NULL);
		if (difftime(current, start) ==10) {
			start = time(NULL);
			ret_val = curl_client();
		}
	} while (1);
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
