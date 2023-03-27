#include "BCMCommunication.h"

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


void command_execution(const char* command){

}

struct sockaddr_in inititate_connection(connection_credentials* cred){
	ssize_t n;
	int i;
    int serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;
	
    // Creating a socket
    cred->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (cred->sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(cred->hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", cred->hostname);
        exit(0);
    }
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0],  
	  (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(cred->portno);

	return serveraddr;
}


