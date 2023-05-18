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


byte* read_packet(int length, connection_credentials* con){
	if (length == 0)
		return NULL;
	byte* buf = (byte*)malloc(sizeof(byte) * length);
	recvfrom(con->sockfd, buf, length, 0, con->serveraddr, sizeof(*con->serveraddr)); 
	debug_printf(buf, length);
	return buf;
}

byte* read_register(commandlist* commands, connection_credentials* con){
	byte* buf = read_packet(commands->message_size, con);
	int* result = (int*)malloc(sizeof(int));
	*result = (buf[2] << 8) | (buf[3] & 0xFF);
	commands->result = result;
	commands->result_size = 1;
	return buf;
}
byte* readADC(commandlist* commands, connection_credentials* con){
	int* result;
	int begin = atoi(commands->args[0]);
	int end = atoi(commands->args[1]);
	int count = end - begin + 1;
	result = (int*)malloc(sizeof(int) * count * 512);
	for (int i = 0; i < count * 512; i++){
		result[i] = 0;
	}
	int j = 0;
	byte* pages = (page*)malloc(sizeof(byte) * count * 1034);
	for (int k = 0; k < count; k++){
		byte* buf = read_packet(commands->message_size, con);
		if (buf == NULL)
			return NULL;
		pages[k] = buf;
		for (int i = 10; i < commands->message_size; j++, i += 2){
			result[j] = ((buf[i] << 8) | (buf[i+1] & 0xFF)) - 2048; 
		}
	}	
	commands->result = result;
	commands->result_size = count * 512;
	return pages;
}

int command_execution(commandlist* commands, connection_credentials* connection){
	byte* buf = NULL;
	int n;
	byte* sendmessage = createmessage(commands);
	n = sendto(connection->sockfd, sendmessage, 6, 0, connection->serveraddr, sizeof(*connection->serveraddr));
	if (n < 0) {
		debug_printf(n, 1);
		return 1;
	}
	buf = read_packet(4, connection);
	if (commands->number == 4){
		buf = (byte*)readADC(commands, connection);
	}
	if (commands->number == 2){
		buf = (byte*)read_register(commands, connection);
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
