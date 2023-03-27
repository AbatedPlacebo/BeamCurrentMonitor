
#include "BCMCommunication.h"
#include "BCMCommandParser.h"


int main(int argc, char* argv[]){
    // Getting ip-address
	connection_credentials* cred;
	cred = parse_ipaddress(argc, argv);
	// Connecting to BCM
	struct sockaddr_in connection = inititate_connection(cred);
	// Executing following commands
	parse_commands(argc, argv);
	return 0;
}
