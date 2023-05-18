#include "BCMCommandParser.h"

commandlist* create_commandlist(int argc, char** argv){
	commandlist* list = NULL;
	commandlist* curlist = NULL;
	for (int i = 0; i < argc; i++){
		for (int j = 0; j < COMM_LIST_COUNT; j++){
			if (strcmp(argv[i], string_commands[j]) == 0){
				commandlist* curlist = create_next_command_node(&list, j);	
				int k = 0;
				for (; k < curlist->args_count; k++){
					curlist->args[k] = argv[i + k + 1];
				}
				i = i + k;
				break;
			}
			else {
				for (int j = 0; j < ADDCOM_LIST_COUNT; j++) {
					int check = strcmp(argv[i], additional_commands[j]);
					if (check == 0){
						int _argc;
						char** _argv;
							switch (j){
							case 0:
							_argc = 0;
							_argv = parse_script(argv[i+1], &_argc);
							list = create_commandlist(_argc, _argv);
							return list;
							break;     
							case 1:
							debug_mode = atoi(argv[i + 1]);
							i++;
							break;
							case 2:
							if (curlist != NULL)
								curlist->output = 1;
							break;
						}
					}
				}
			}
		}
	}
	return list;
}

commandlist* parse_commands(int argc, char* argv[]){
	return create_commandlist(argc - 2, argv + 2);
}

int count_file_length(FILE* file){
	int length = -1;
	if (file == NULL)
		return -1;
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
		return NULL;
	}
	FILE* file = fopen(filename, "r");
	if (file == NULL)
		return NULL;
	int length = count_file_length(file);
	char** argv = create_command_array(file, length, words);
	return argv;
}


connection_credentials* parse_ipaddress(int argc, char* argv[]){
	if (argc < 2) {
		printf(args_message);
		exit(1);
	}
	connection_credentials* cred;
	cred = (connection_credentials*)malloc(sizeof(connection_credentials));
	cred->hostname = argv[1];
	cred->portno = 2195;
	return cred;
}
