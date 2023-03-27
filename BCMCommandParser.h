// Parser header

#include <stdlib.h>
#include <stdio.h>
#include "BCMConnection.h"

char** parse_commands(int argc, char* argv[]);

char** parse_script(const char* filename);

connection_credentials parse_ipaddress(int argc, char* argv[]);
