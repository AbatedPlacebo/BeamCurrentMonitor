all:
	gcc -std=c99 main.c BCMCommunication.c BCMCommandParser.c -o main -g -ggdb
