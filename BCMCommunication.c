#include "BCMCommunication.h"

void command_execution(const char* command){

}

sockaddr_in inititate_connection(connection_credentials* cred){
	ssize_t n;
	int i;
    int serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;
	
    // Creating a socket
    cred->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(cred->hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", cred->hostname);
        exit(0);
    }
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
	  (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(cred->portno);

	return serveraddr;
}


