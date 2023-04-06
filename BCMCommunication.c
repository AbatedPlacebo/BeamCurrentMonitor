#include "BCMCommunication.h"

byte* createmessage(commandlist* commands){
	byte* array = (byte*)malloc(sizeof(byte)*6);
	int command = commands->number;
	array[0] = inputcommands[command];
	for (int i = 1; i < 6; i++)
		array[i] = 0x00;
	if (command == 0 || command == 2){
		array[1] = atoi(commands->args[0]);
	}
	if (command == 4){
		int firstpage = atoi(commands->args[0]);
		int secondpage = atoi(commands->args[1]);
		array[2] = firstpage >> 8;
		array[3] = firstpage & 0xFF; 
		array[4] = secondpage >> 8;
		array[5] = secondpage & 0xFF; 

	}
	if (command == 0){
		int data = atoi(commands->args[1]);
		array[2] = data >> 8;
		array[3] = data & 0xFF; 
	}
	return array;
}

void read_ack(int length, int sockfd, struct sockaddr_in serveraddr, int serverlen){
	int n;
	byte buf[length];
	n = recvfrom(sockfd, buf, 4, 0, &serveraddr, &serverlen);
	if (n < 0){
		error("ERROR in recvfrom\n");
		return;
	}
	#ifdef DEBUG_INFO
	printf("ACK from server:\n");
	int i;
	for (i = 0; i<4;i++){
		printf("%#04X\n",buf[i]);
	}
	#endif
}

void read_packet(int length, int sockfd, struct sockaddr_in serveraddr, int serverlen){
	byte buf[length];
	if (length == 0)
		return;
	int	n = recvfrom(sockfd, buf, length, 0, &serveraddr, &serverlen);
	if (n < 0){
		error("ERROR in recvfrom\n");
		return;
	}
	int i = 0;
	#ifdef DEBUG_INFO
	printf("PACK from server:\n");
	#else
	i = 10;
	#endif
	#ifdef DEBUG_INFO
	for (; i<length;i++){
		printf("%#04X\n",buf[i]);
	}
	#else
	for (; i<length/2;i+=2){
		int res = ((buf[i] * 256) + buf[i+1]) - 2048;
		printf("%d\n",res);
	}
	#endif
	return buf;
}


void readADC(commandlist* commands, int length, int sockfd, struct sockaddr_in serveraddr, int serverlen){
	page pages[128];
	int begin = atoi(commands->args[0]);
	int end = atoi(commands->args[1]);
	int count = end - begin + 1;
	for (int i = 0; i < count; i++){
		read_packet(length, sockfd, serveraddr, serverlen);
	}
}

void command_execution(commandlist* commands, connection_credentials* connection){
	int serverlen, n;
	serverlen = sizeof(*(connection->serveraddr));
	while (commands != NULL){
		byte* sendmessage = createmessage(commands);
		n = sendto(connection->sockfd, sendmessage, 6, 0, connection->serveraddr, serverlen);
		if (n < 0) 
			error("ERROR in sendto");
		read_ack(packetlengths[commands->number], connection->sockfd, *(connection->serveraddr), serverlen);
		if (commands->number == 4)
			readADC(commands, packetlengths[commands->number], connection->sockfd, *(connection->serveraddr), serverlen);
		else
			read_packet(packetlengths[commands->number], connection->sockfd, *(connection->serveraddr), serverlen);
		commands = commands->next;
	}
}


void initiate_connection(connection_credentials* cred){
	ssize_t n;
	int i;
	int serverlen;
	struct sockaddr_in* serveraddr;
	struct hostent *server;

	// Sockaddr allocating
	serveraddr = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));

	// Creating a socket
	cred->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (cred->sockfd < 0) 
		error("ERROR opening socket");
	server = gethostbyname(cred->hostname);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host as %s\n", cred->hostname);
		exit(0);
	}
	bzero((char *)serveraddr, sizeof(*serveraddr));
	serveraddr->sin_family = AF_INET;
	bcopy((char *)server->h_addr_list[0],  
			(char *)&(serveraddr->sin_addr.s_addr), server->h_length);
	serveraddr->sin_port = htons(cred->portno);

	cred->serveraddr = serveraddr;
}


