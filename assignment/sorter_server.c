#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
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
	
	int s_sock, c_sock, tmp;
	
	struct sockaddr_in adr, c_adr;

	
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

	
	if (listen(s_sock, 100) < 0)
	{

		perror("listen");

		close(s_sock);

		exit(EXIT_FAILURE);
	}

	printf("Waiting for connections...\n");
	 tmp = sizeof(struct sockaddr_in);

	int i = 0;
    	struct sockaddr_storage stor_adr;
    	socklen_t len;
	//inet6 size value i think its 46
	    char ip_val[INET6_ADDRSTRLEN];
	    int port2;
	pthread_t tid[1000];
	
	while (1)
	{
		//store client ip info in c_adr
		c_sock = accept(s_sock,  (struct sockaddr *)&c_adr, (socklen_t *)&tmp);
		
		if (c_sock < 0)
		{

			perror("accept");

			close(s_sock);

			exit(EXIT_FAILURE);
		}
	printf("----port %d ----ip %s\n",ntohs(c_adr.sin_port), inet_ntoa(c_adr.sin_addr));
		  len = sizeof(stor_adr);
        getpeername(c_sock, (struct sockaddr *)&stor_adr, &len);
        struct sockaddr_in *s = (struct sockaddr_in*) &stor_adr;
	port2 = ntohs(s -> sin_port);
	inet_ntop(AF_INET, &s->sin_addr, ip_val, sizeof ip_val);
        printf("Client IP Address: (%s)\n", ip_val);
	printf("Client Port      : (%d)\n", port2);

	char rec[256];
	read(c_sock,rec,256);
	printf("reading - %s\n",rec);
	
	}

	/* clean up */
	close(s_sock);
	close(c_sock);
	return 0;




}


