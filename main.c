#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

typedef unsigned char byte;

enum COMMANDS {
	WRITE_RGSTR = 0x00;
	START_CYCLE = 0x03;
	READ_PARAM = 0x04;
	RESET_CYCL = 0x05;
	INIT_GENER = 0x06;
	RESET_COUNT = 0x07;
	READ_ADCBUF = 0x08;
	WRITE_FLASH = 0x09;
	REWRITE_CONF = 0x0a;
	READ_FLASH = 0x0f;
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
}	

void printmenu(){
		printf("0 - EXIT\n"
				"1 - WRITE REGISTER\n"
				"2 - START CYCLE\n"
				"3 - READ PARAMETER\n"
				"4 - RESET CYCLE\n"
				"5 - INIT GENERATOR\n"
				"6 - RESET COUNTER\n"
				"7 - READ ADC BUFFER\n"
				"8 - WRITE FLASH PARAMETERS\n"
				"9 - REWRITE CONFIGURATIONS\n"
				"10 - READ FLASH PARAMETERS\n");
}

void error(char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char **argv) {
    int sockfd, portno, n;
	int i;
    int serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char* hostname;
    byte buf[100];
	byte sendmessage[6];
    /* check command line arguments */
    if (argc != 3) {
       fprintf(stderr,"usage: %s <hostname> <port>\n", argv[0]);
       exit(0);
    }
    hostname = argv[1];
    portno = atoi(argv[2]);

    /* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }

    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
	  (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);
	// Communication cycle begins
	while (1){
		int command;
		printf("Enter a command: ");
		printmenu();
		scanf("%d", &command);
		
		/* sends the message to the server */
		serverlen = sizeof(serveraddr);
		sendmessage[0] = inputcommands[command];
		sendmessage[1] = 0x00;
		sendmessage[2] = 0x00;
		sendmessage[3] = 0x00;
		sendmessage[4] = 0x00;
		sendmessage[5] = 0x00;
			printf("sending message:\n");
		for (i = 0; i<6;i++){
			printf("%d\n",sendmessage[i]);
		}
		n = sendto(sockfd, sendmessage, 6, 0, &serveraddr, serverlen);
		if (n < 0) 
		  error("ERROR in sendto");

		/* print the server's reply */
		n = recvfrom(sockfd, buf, 4, 0, &serveraddr, &serverlen);
		if (n < 0) 
		  error("ERROR in recvfrom");
		printf("Echo from server:\n");
		for (i = 0; i<6;i++){
			printf("%d\n",buf[i]);
		}
	}
	return 0;
}
