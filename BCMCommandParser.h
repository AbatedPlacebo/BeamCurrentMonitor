// Parser header
#ifndef BCM_COMMANDPARSER_H
#define BCM_COMMANDPARSER_H 

#include "BCMVariables.h"
#include <stdlib.h>
#include <stdio.h>

char** parse_commands(int argc, char* argv[]);

char** parse_script(const char* filename);

connection_credentials* parse_ipaddress(int argc, char* argv[]);
#endif
