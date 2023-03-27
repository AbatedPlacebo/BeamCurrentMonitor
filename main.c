#include "BCMConnection.h"
#include "BCMCommandParser.h"

int main(int argc, char* argv[]){
    // Getting ip-address
	connection_credentials cred;
	cred = parse_ipaddress(argv);
	sockaddr_in connection = inititate_connection(cred);
	return 0;
}
