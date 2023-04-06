#include "BCMVariables.h"

enum COMMANDS {
	WRITE_RGSTR = 0x00,
	START_CYCLE = 0x03,
	READ_PARAM = 0x04,
	RESET_CYCL = 0x05,
	INIT_GENER = 0x06,
	RESET_COUNT = 0x07,
	READ_ADCBUF = 0x08,
	WRITE_FLASH = 0x09,
	REWRITE_CONF = 0x0a,
	READ_FLASH = 0x0f,
};

int inputcommands[] = {
	0x00,
	0x03,
	0x04,
	0x05,
	0x08,
	0x06,
	0x07,
	0x09,
	0x0a,
	0x0f
};	

int packetlengths[] = {
	0,
	2,
	4,
	0,
	1034,
	0,
	4,
	2,
	0,
	4,
	0
};


int command_args_num[] = {
	2,
	0,
	1,
	0,
	2
};

char* args_message = "example: \"ip-address port command [args] ...\"\n";

char* string_commands[] = {
	"writeregs",
	"start",
	"readregs",
	"stop",
	"readbuffer"
};


commandlist* init_commandlist(){
	commandlist* list = (commandlist*)malloc(sizeof(commandlist));
	list->next = NULL;
	return list;
}

void create_next_command_node(commandlist** curlist, int _commandnumber, char** _commandargs){
	commandlist* seekelem = *curlist;
	commandlist* list = init_commandlist(); 
	list->number = _commandnumber;
	list->args = _commandargs; 
	if (seekelem == NULL){
		*curlist = list;
	}
	else {
		while (seekelem->next != NULL)
			seekelem = seekelem->next;
		seekelem->next = list;
	}
}

void error(char *msg) {
    perror(msg); 
	exit(0);
}

void my_strcpy(char** dest, const char* source){
	*dest = (char*)malloc(strlen(source) * sizeof(char) + 1);
	strcpy(*dest, source);
}



