all:
	gcc -std=c99 main.c BCMCommunication.c BCMCommandParser.c BCMVariables.c -o main -g -ggdb
