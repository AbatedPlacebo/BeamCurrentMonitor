// Parser header
#ifndef BCM_COMMANDPARSER_H
#define BCM_COMMANDPARSER_H 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "BCMVariables.h"

commandlist* parse_commands(int argc, char* argv[]);

char** parse_script(const char* filename, int* words);

connection_credentials* parse_ipaddress(int argc, char* argv[]);

#endif
