#ifndef BCM_VARIABLES_H
#define BCM_VARIABLES_H

typedef unsigned char byte;
typedef byte page[1034];

typedef struct _connection_credentials {
	char* hostname;
	int portno, sockfd;
} connection_credentials;

extern enum COMMANDS;

extern int inputcommands[]; 

extern int packetlengths[]; 

extern char* args_message;

#endif
