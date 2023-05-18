#include "BCMIO.h"

void file_log(commandlist* commands){
	if (commands->output != 1) return;
	FILE* file = fopen(commands->file_name, "w");
	if (file == NULL) return;
	for (int i = 0; i < commands->result_size; i++)
		fprintf(file,"%d\n",commands->result[i]);
	fclose(file);
}
