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

int* message_conversion(void* info, size_t size){
	switch(size){
		case sizeof(page):

		break;
		case sizeof(byte) * 4:
		break;
	}
}

byte* read_packet(int length, connection_credentials* con){
	if (length == 0)
		return NULL;
	byte* buf = (byte*)malloc(sizeof(byte) * length);
	int	n = recvfrom(con->sockfd, buf, length, 0, con->serveraddr, sizeof(*con->serveraddr));
	if (n < 0){
		return NULL;
	}
	int i = 0;
	debug_printf(buf, length);
	return buf;
}

page* readADC(commandlist* commands, connection_credentials* con){
	int begin = atoi(commands->args[0]);
	int end = atoi(commands->args[1]);
	int count = end - begin + 1;
	page* pages = (page*)malloc(sizeof(page) * count);
	for (int k = 0; k < count; k++){
		byte* buf = read_packet(commands->message_size, con);
		if (buf == NULL)
			return NULL;
	}	
	return pages;
}

int command_execution(commandlist* commands, connection_credentials* connection){
	byte* buf = NULL;
	int n;
	byte* sendmessage = createmessage(commands);
	n = sendto(connection->sockfd, sendmessage, 6, 0, connection->serveraddr, sizeof(*connection->serveraddr));
	if (n < 0) 
		return 1;
	read_packet(4, connection);
	if (commands->number == 4){
		buf = (byte*)readADC(commands, connection);
	}
	else {
		buf = read_packet(commands->message_size, connection);
	}
	return 0;
}

int initiate_connection(connection_credentials* cred){
	ssize_t n;
	int i;
	int serverlen;
	struct sockaddr_in* serveraddr;
	struct hostent *server;

	// Sockaddr allocating
	serveraddr = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));

	// Creating a socket
	cred->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	setsockopt(cred->sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
	if (cred->sockfd < 0) 
		// Error while opening a socket
		return 1;
	server = gethostbyname(cred->hostname);
	if (server == NULL) 
		// No such host as hostname
		return 2;
	bzero((char *)serveraddr, sizeof(*serveraddr));
	serveraddr->sin_family = AF_INET;
	bcopy((char *)server->h_addr_list[0],  
			(char *)&(serveraddr->sin_addr.s_addr), server->h_length);
	serveraddr->sin_port = htons(cred->portno);

	cred->serveraddr = serveraddr;
	return 0;
}
