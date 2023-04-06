#ifndef BCM_COMMUNICATION_H
#define BCM_COMMUNICATION_H 

#include "BCMVariables.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

// Initiating connection between the host and BCM
void initiate_connection(connection_credentials* cred);

// Executing command
void command_execution(commandlist* commands, connection_credentials* connection);

#endif
