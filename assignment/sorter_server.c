#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>



int main (int argc, char* argv[]){
char* charport;
charport = (char*)malloc(sizeof(char*)*100);
int port_num;
int x = 0;
while(argv[x]){
x++;
}

if(x>3 || x<3){

	printf("Not the right amount of arguments\n");
	exit(0);
}


if(strcmp(argv[1],"-p")==0){

	charport=strcpy(charport,argv[2]);

}else{

	printf("Enter valid argument\n");
	exit(0);
}

port_num = atoi(charport);

printf("--%d\n",port_num);
	
	int s_sock, c_sock;
	
	struct sockaddr_in adr;
cd re
	
	if ((s_sock = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
	{

		perror("socket");

		exit(EXIT_FAILURE);
	}

	
	adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = INADDR_ANY;
	adr.sin_port = htons(port_num);
	
	if (bind(s_sock, (struct sockaddr*)&adr, sizeof(adr)) < 0)
	{

		perror("bind");

		close(s_sock);

		exit(EXIT_FAILURE);
	}

	
	if (listen(s_sock, 0) < 0)
	{

		perror("listen");

		close(s_sock);

		exit(EXIT_FAILURE);
	}

	printf("Waiting for connections...\n");

	
	while (1)
	{
		c_sock = accept(s_sock, NULL, NULL);

		if (c_sock < 0)
		{

			perror("accept");

			close(s_sock);

			exit(EXIT_FAILURE);
		}


		printf("Connect to client %d\n", c_sock); 

	
	}

	/* clean up */
	close(s_sock);
	return 0;




}


