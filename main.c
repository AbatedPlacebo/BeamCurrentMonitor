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
	0
};

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

//void readADC(int sockfd, struct sockaddr_in serveraddr, int serverlen){
//	int i, n;
//	for (i = 0; i < 127; i++){
//		n = recvfrom(sockfd, pages[i], 1034, 0, &serveraddr, &serverlen);
//	}
//	
//}

void read_packet(int length, int sockfd, struct sockaddr_in serveraddr, int serverlen){
	int n;
    byte buf[length];
	page pages[128];
	
	n = recvfrom(sockfd, buf, 4, 0, &serveraddr, &serverlen);
	if (n < 0){
		error("ERROR in recvfrom\n");
		return;
	}
	printf("ACK from server:\n");
	int i;
	for (i = 0; i<4;i++){
		printf("%#04X\n",buf[i]);
	}
	if (length == 0)
		return;
	n = recvfrom(sockfd, buf, length, 0, &serveraddr, &serverlen);
	if (n < 0){
		error("ERROR in recvfrom\n");
		return;
	}
	printf("PACK from server:\n");
	for (i = 0; i<length;i++){
		printf("%#04X\n",buf[i]);
	}
	printf("\n");
}

void createmessage(byte* array, int command){
	array[0] = inputcommands[command-1];
	array[1] = 0x00;
	array[2] = 0x00;
	array[3] = 0x00;
	array[4] = 0x00;
	array[5] = 0x00;
	int data = 0;
	int regist = 0;	
	int firstpage = 0;
	int secondpage = 0;
	if (command == 1 || command == 3){
		printf("Enter register's number 0-31: ");
		scanf("%d",&regist);
		array[1] = (byte)regist;
	}
	if (command == 7){
		printf("Enter first page number 0-127: ");
		scanf("%d",&firstpage);
		printf("Enter second page number 0-127: ");
		scanf("%d",&secondpage);
		array[2] = firstpage >> 8;
		array[3] = firstpage & 0xFF; 
		array[4] = secondpage >> 8;
		array[5] = secondpage & 0xFF; 
		
	}
	if (command == 1){
		printf("Enter data 0-511: ");
		scanf("%d",&data);
		array[2] = data >> 8;
		array[3] = data & 0xFF; 
	}
}

void error(char *msg) {
    perror(msg); 
	exit(0);
}

int main(int argc, char **argv) {
    int sockfd, portno;
	ssize_t n;
	int i;
    int serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char* hostname;
	//bool cycle = false; 
	
	byte sendmessage[6];
    hostname = "192.168.1.9";
    portno = 2195;

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
		int command = 0;
		printmenu();
		printf("Enter a command: ");
		scanf("%d", &command);
 		if (command == 0)
			break;
		createmessage(sendmessage,command);
		/* sends the message to the server */
		printf("sending message:\n");
		for (i = 0; i<6;i++){
			printf("%d\n",sendmessage[i]);
		}
		char confirm;
		while (1){
			printf("Are you sure? [y/n]\n");
			scanf(" %c", &confirm);
			if ((confirm == 'n') || (confirm == 'N') ||
			(confirm == 'y') || (confirm == 'Y') ){
				break;
				}	
			else {
				printf("Please, try again\n");
			}
		}
		if (confirm == 'n' || confirm == 'N')
			continue;
		serverlen = sizeof(serveraddr);
		n = sendto(sockfd, sendmessage, 6, 0, &serveraddr, serverlen);
		if (n < 0) 
		  error("ERROR in sendto");
		read_packet(packetlengths[command-1], sockfd, serveraddr, serverlen);
	}
	return 0;
}
