#include "BCMCommandParser.h"

char* parse_commands(int argc, char* argv[]){
	char commands[7][40];
	for (int i = 1; i < argc; i++){
		commands[i-1] = argv[i];
	}
}

char** parse_script(const char* filename){
	FILE* file = fopen(filename, "r");
}

connection_credentials parse_ipaddress(int argc, char* argv[]){
	connection_credentials cred;
	cred.hostname = argv[1];
	cred.portno = argv[2];
	return cred;
}
