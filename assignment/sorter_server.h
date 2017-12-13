#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <dirent.h>
#include <unistd.h>
#include <pthread.h>

/* Socket Stuff */
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

#ifndef SORTER_SERVER_H
	#define SORTER_SERVER_H

	struct store {
		char* dirname;
	};

	typedef struct {
	    char** rToken;
	    char * rText;
	    size_t rowL;
	    size_t numc;
	} row;

	void mergeSort(row* target, int col, int n);
	int isNum(char* string);
	void sorter(void* arg);
	void openDir(void* arg);
#endif