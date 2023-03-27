#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

typedef unsigned char byte;
typedef byte page[1034];

typedef struct connection_credentials{
	char* hostname;
	int portno, sockfd;
} connection_credentials;

enum COMMANDS {
	WRITE_RGSTR = 0x00,
	START_CYCLE = 0x03,
	READ_PARAM = 0x04,
	RESET_CYCL = 0x05,
	INIT_GENER = 0x06,
	RESET_COUNT = 0x07,
	READ_ADCBUF = 0x08,
	WRITE_FLASH = 0x09,
	REWRITE_CONF = 0x0a,
	READ_FLASH = 0x0f,
};

int inputcommands[] = {
	0x00,
	0x03,
	0x04,
	0x05,
	0x06,
	0x07,
	0x08,
	0x09,
	0x0a,
	0x0f
};	

int packetlengths[] = {
	0,
	2,
	4,
	0,
	0,
	4,
	1034,
	2,
	0,
	4,
	0
};

// Initiating connection between the host and BCM
int inititate_connection(connection_credentials* cred);

// Executing command
void command_execution(const char* command);

