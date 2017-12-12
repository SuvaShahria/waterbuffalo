
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


#define PORT 9000
#define ADDR 127.0.0.1

int main(int argc, char **argv)
{
	
	int sockfd;

	int port;
	
	struct sockaddr_in address;
	// We need a hostent struct pointer to 
	// hold server info from gethostbyname()
	struct hostent *server;

	// We need a buffer for messages
	char buffer[256];

	if (argc < 3)
	{
		fprintf(stderr, "usage %s hostname portnum\n", argv[0]);
		exit(0);
	}

	// initialize portnum variable with argv[2]
	// you can use atoi for this demo but atoi 
	// is easy to fail, not a good choice in general
	port = atoi(argv[2]);
	// initialize hostent pointer by calling 
	// gethostbyname(argv[1])
	

/*server = gethostbyname(argv[1]);
	// replace hostent pointer with your pointer name
	if (server == NULL)
	{
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

	/*bcopy((char*)server->h_addr,
			(char*)&address.sin_addr.s_addr,
			server->h_length);
					*/

	/****** STEP 1: Create Socket ******/
	// call socket(AF_INET, SOCK_STREAM, 0) to create socket	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		//perror() report error message
		perror("sock");
		//exit your program
		exit(EXIT_FAILURE);
	}

	/****** STEP 2: Connection *******/
	// call connect(socketfd, (struct sockaddr*)&..., sizeof(...))
	if (connect(sockfd, (struct sockaddr*)&address, sizeof(address)) < 0)
	{
		// perror() report error message
		perror("connect");
		// close socket
		close(sockfd);
		// exit your program
		exit(EXIT_FAILURE);
	}
	printf("Please enter your message: ");
	// You can use fgets() in this demo to receive user input
	fgets(buffer, 256, stdin);
	// call write to send data
	int n = write(sockfd, buffer, 256);
	// check if message is sent successfully
	
	if (n <= 0)
	{
		// perror() again...
		perror("write");
	}

	// call read to receive data from server
	/*memset(buffer, 0, 256);
	n = read(sockfd, buffer, 256);
	if (n < 0)
	{
		// you know what to do
		perror("read");
	}

	printf("from server: %s\n", buffer);*/
	
	/* clean up: don't forget to close socket */
	close(sockfd);

	return 0;
}
