#include "BCMCommandParser.h"

char* args_message = "example: [ip-address] [port] [command] [args] ...\n";

char** parse_commands(int argc, char* argv[]){
	char* commands[7];
	for (int i = 1; i < argc; i++){
		commands[i-1] = argv[i];
	}
}

char** parse_script(const char* filename){
	FILE* file = fopen(filename, "r");
}

connection_credentials* parse_ipaddress(int argc, char* argv[]){
	if (argc < 3) {
		printf(args_message);
		exit(1);
	}
	connection_credentials* cred;
	cred = (connection_credentials*)malloc(sizeof(connection_credentials));
	cred->hostname = argv[1];
	cred->portno = atoi(argv[2]);
	return cred;
}
