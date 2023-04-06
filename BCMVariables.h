// Variables
#ifndef BCM_VARIABLES_H
#define BCM_VARIABLES_H

#include <netinet/in.h>
#include <stdlib.h>
#include <assert.h>

#define COMM_LIST_SIZE 5

typedef unsigned char byte;
typedef byte page[1034];

typedef struct _connection_credentials {
	char* hostname;
	int portno, sockfd;
	struct sockaddr_in* serveraddr;
} connection_credentials;

typedef struct _commandlist {
	int number;
	char** args;
	struct _commandlist* next;
} commandlist;

extern enum COMMANDS;

extern int inputcommands[]; 

extern int packetlengths[]; 

extern char* args_message;

extern char* string_commands[];

extern int command_args_num[];

commandlist* init_commandlist();

void create_next_command_node(commandlist** curlist, int _commandnumber, char** _commandargs);

void error(char* msg);

void my_strcpy(char** dest, const char* source);

#endif
