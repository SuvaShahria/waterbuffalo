
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

/* Socket Stuff */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

/* others */
#include <sys/types.h>


//doing this in place of a header file for now
void goThroughDir(char dirName[]);
int isCSV(char filename[]);
int okayDir(char filename[]);



#define PORT 9000
#define ADDR 127.0.0.1

char outputDirName[1000];
char sortColumn[1000];

int main(int argc, char *argv[]) {
	
	int sockfd;
	int port;
	struct sockaddr_in address;
	struct hostent *server;			// We need a hostent struct pointer to hold server info from gethostbyname()
	char buffer[256];				// We need a buffer for messages
	char readDirName[1000];

	//checking arguments
	if (argc < 7) {
		printf("invalid arguments to run\n");
		return 0;
	}

	strcpy(sortColumn, argv[2]);
	port = atoi(argv[6]);

	if (argc == 9) {
		if (strcmp(argv[7], "-d") == 0) {
			strcpy(readDirName, argv[8]);
		}
		else if (strcmp(argv[7], "-o") == 0) {
			strcpy(outputDirName, argv[8]);
		}
	}

	if (argc == 11) {
		if (strcmp(argv[7], "-d") == 0) {
			strcpy(readDirName, argv[8]);
			strcpy(outputDirName, argv[10]);
		}
		else if (strcmp(argv[7], "-o") == 0) {
			strcpy(outputDirName, argv[8]);
			strcpy(readDirName, argv[10]);
		}
	}

	// initialize hostent pointer by calling 
	// gethostbyname(argv[1])
	
	/*
	server = gethostbyname(argv[1]);
	// replace hostent pointer with your pointer name
	if (server == NULL) {
		// you can also call h_error() instead
		fprintf(stderr, "ERROR, NO SUCH HOST\n");
		exit(0);
	}
	*/

	// Now we need to initialize sockaddr_in struct
	// before we do initialization, it's good to set buffer zeros
	// replace corresponding parts
	memset(&address, 0, sizeof(address));
	// .sin_family = AF_INET
	address.sin_family = AF_INET;
	// .sin_port = htons(portnumber)
	address.sin_port = htons(port);
	address.sin_addr.s_addr =inet_addr(argv[1]);
	
	// replace corresponding parts
	// we do binary copy to initialize sockaddr_in address part
	// from hostent pointer

	/*
	bcopy((char*)server->h_addr, (char*)&address.sin_addr.s_addr, server->h_length);
	*/

	/****** STEP 1: Create Socket ******/
	// call socket(AF_INET, SOCK_STREAM, 0) to create socket	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("sock");
		exit(EXIT_FAILURE);
	}

	/****** STEP 2: Connection *******/
	// call connect(socketfd, (struct sockaddr*)&..., sizeof(...))
	if (connect(sockfd, (struct sockaddr*)&address, sizeof(address)) < 0) {
		perror("connect");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	// You can use fgets() in this demo to receive user input
	printf("Please enter your message: ");
	fgets(buffer, 256, stdin);
	
	// call write to send data
	int n = write(sockfd, buffer, 256);
	
	// check if message is sent successfully
	if (n <= 0) {
		perror("write");
	}

	// call read to receive data from server
	/*
	memset(buffer, 0, 256);
	n = read(sockfd, buffer, 256);
	if (n < 0) {
		perror("read");
	}

	printf("from server: %s\n", buffer);*/
	
	/* clean up: don't forget to close socket */
	close(sockfd);
	
	return 0;
}

/*
Function to traverse through a directory, and all subdirectories.
Each new CSV found, a thread is created and operation goes to the thread.
Each new subdirectory found is stored in an array, and recursively 
called upon at the end of the function.

params: dirName 	name of directory to traverse
return: void
*/
void goThroughDir(char dirName[]) {
	DIR *dir;
	struct dirent *entry;
	char newDirs[256][750];
	int numDirs = 0;

	if ((dir = opendir(dirName)) != NULL) {
		while ((entr = readdir(dir)) != NULL) {
			char entryName[256];
			strcpy(entryName, entry->d_name);

			if (isCSV(entryName)) {
				//going to this part in a bit
				//supposed to create thread, start function
				//to send whole CSV to server
			}

			else if (okayDir(entryName)) {
				strcat(newDirs[numDirs], dirName);
				strcat(newDirs[numDirs], "/");
				strcat(newDirs[numDirs], entryName);
				numDirs++;
			}
		}

		closedir(dir);
	}

	int i;
	for (i = 0; i < numDirs; i++) {
		goThroughDir(newDirs[i]);
	}
}

/*
Function to check if a given file is a CSV.

params: filename	name of the file
return:	1 if the file is a CSV
		0 if the file isn't a CSV
*/
int isCSV(char filename[]) {
	int l = strlen(filename);

	if (l < 4) {
		return 0;
	}

	if (filename[l - 1] == 'v' && filename[l - 2] == 's' && filename[l - 3] == 'c') {
		return 1;
	}

	return 0;
}

/*
Function to check if a given string is a directory.

params:	filename	name of possible directory
return:	0 if it's a bad directory to traverse
		1 if it's okay
*/
int okayDir(char filename[]) {
	if (filename[0] == '.' && filename[1] == '.') {
		return 0;
	}

	if (filename[0] == '.') {
		return 0;
	}

	return 1;
}

