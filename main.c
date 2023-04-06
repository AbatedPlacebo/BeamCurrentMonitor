#include "BCMCommunication.h"
#include "BCMCommandParser.h"


int main(int argc, char* argv[]){
    // Getting ip-address
	connection_credentials* cred;
	cred = parse_ipaddress(argc, argv);
	// Connecting to BCM
	initiate_connection(cred);
	// Executing following commands
	commandlist* commands = parse_commands(argc, argv);
	command_execution(commands, cred);
	return 0;
}
