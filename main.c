#include "BCMCommunication.h"

int main(int argc, char* argv[]){
    // Getting ip-address
	connection_credentials* cred;
	cred = parse_ipaddress(argc, argv);
	// Connecting to BCM
	initiate_connection(cred);
	// Parsing commands
	commandlist* commands = parse_commands(argc, argv);
	while (commands != NULL){
		// Executing following commands
		command_execution(commands, cred);
		file_log(commands);
		commands = commands->next;
	}
	return 0;
}
