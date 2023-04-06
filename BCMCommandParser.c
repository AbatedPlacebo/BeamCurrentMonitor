#include "BCMCommandParser.h"


commandlist* create_commandlist(int argc, char** argv, int index){
	commandlist* list = NULL;
	for (int i = index; i < argc; ){
//		char* command = argv[i];
		for (int j = 0; j < COMM_LIST_SIZE; j++){
			if (strcmp(argv[i], string_commands[j]) == 0){
				i++;
				char** args = (char**)malloc(sizeof(char*) * command_args_num[j]);
				for (int k = 0; k < command_args_num[j]; i++, k++){
					args[k] = argv[i];
				}
				create_next_command_node(&list, j, args);	
				break;
			}
		}
	}
	return list;
}

commandlist* parse_commands(int argc, char* argv[]){
	commandlist* list = NULL;
	int val = strcmp(argv[3],"-S"); 
	if (argc == 5 && val == 0) {
		int _argc = 0;
		char** _argv = parse_script(argv[4], &_argc);
		list = create_commandlist(_argc, _argv,0);
	}
	else {
		list = create_commandlist(argc, argv,3);
	} 
	return list;
}

int count_file_length(FILE* file){
	int length = -1;
	if (file == NULL)
		error("File is not opened\n");	
	for (char c = getc(file); c != EOF; c = getc(file)){
		length = length + 1;
	}
	fseek(file, 0, SEEK_SET);
	return length;
}

char** create_command_array(FILE* file, int length, int* spaces){
	char** commands;
	char filetext[length]; 
	const char* delim = " ";
	int i = 0;
	*spaces = 1;
	for (char c = getc(file); i < length; i++, c = getc(file)){
		if (c == '\n'){
			filetext[i] = ' ';
			*spaces = *spaces + 1;
		}
		else {
			if (c == ' ')
				*spaces = *spaces + 1;
			filetext[i] = c;
		}

	}
	fclose(file);
	filetext[i] = '\0';
	i = 0;
	commands = (char**)malloc(sizeof(char*) * (*spaces));
	char* token = strtok(filetext, delim);
	my_strcpy(&commands[i++], token);
	while( token != NULL ) {
		token = strtok(NULL, delim);
		if (token != NULL)
			my_strcpy(&commands[i++], token);
	}
	return commands;
}

char** parse_script(const char* filename, int* words){
	if (strstr(filename, ".bcm") == NULL){
		error("Incorrect file type. use .bcm extension\n");
	}
	FILE* file = fopen(filename, "r");
	if (file == NULL)
		error("File is not opened\n");	
	int length = count_file_length(file);
	char** argv = create_command_array(file, length, words);
	return argv;
}


connection_credentials* parse_ipaddress(int argc, char* argv[]){
	if (argc < 3) {
		printf(args_message);
		exit(1);
	}
	connection_credentials* cred;
	cred = (connection_credentials*)malloc(sizeof(connection_credentials));
	cred->hostname = argv[1];
	cred->portno = atoi(argv[2]);
	return cred;
}
