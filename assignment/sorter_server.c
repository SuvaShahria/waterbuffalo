#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "sorter_thread.h"







char** tokenizer(char* tp, size_t numc){
    

    int i = 0;
    int j = 0;
    int k = 0;
    
    char** str =(char**)malloc(sizeof(char*)* (numc + 1)); 
    char* tmp =(char*)malloc(sizeof(char*)*500);
	
size_t quote=1;
	

    while(i<strlen(tp)){

		if(tp[i]=='"' && quote==1){
			quote=0;

		
		}
				
		else if(tp[i]=='"' && quote==0){
			
			str[k] = (char*)malloc((100)* sizeof(char));
			int x =j-1;
			
			char* end;



			strcpy(str[k],tmp);

			memset(&tmp[0],0,strlen(tmp));
			quote=1;
			j=0;
			k++;
			i++;
		}

		
        else if((tp[i]==',' || i ==strlen(tp)-1) && quote==1){
            
            if(!tmp){
                tmp[0] = '\0';
			}
			if(i == strlen(tp)-1 && tp[i]!='\n' && tp[1] != ','){
				tmp[j] = tp[i];
				j++;
			}
           
			str[k] = (char*)malloc((j+1) * sizeof(char));

//wer
		
			int x = j-1;
			char *txt = tmp;
			char *end;
			while(isspace(*txt)){
			txt++;
						
			
		}

if(*txt==0){
tmp =txt;
}else{
end=txt+x;
while(end>txt && isspace(*end)){
end--;
}
end++;
*(end)=0;
tmp=txt;
}




			strcpy(str[k],tmp);
			memset(&tmp[0],0,strlen(tmp));			
            j = 0;
			k++;
			if(tp[1]==',' && i==strlen(tp)-1){
			tmp[0]='\0';
			str[k]= (char*)malloc((j+1)*sizeof(char));
			strcpy(str[k],tmp);
			memset(&tmp[0],0,strlen(tmp));
			}


        }else{

			if(j==0){
				if(tp[i]==' '){
					i++;
					continue;
				}				
			}
            tmp[j] = tp[i];
			j++;
		}
        i++;
	} 

	free(tmp);

    return str;
}




void sorter(void* arg){

int* c_socket;
c_socket = (int*) arg;
int cs = *c_socket;
char rec[256];

int colnumber = 0;
	int tmp= 0;
	read(cs,&tmp,sizeof(tmp));
	colnumber = ntohl(tmp); // which col number to sort

	//printf("this is %d\n",colnumber);

	long bsize = 0;
	long tmpbs = 0;
	read(cs,&tmpbs,sizeof(tmpbs));
	bsize = ntohl(tmpbs); // size of buffer
	char buffer[bsize+1]; // make buffer
	bzero(buffer,bsize);
		read(cs,buffer,bsize);
	buffer[bsize] = '\0';


	char** store = (char**)malloc(sizeof(char*) * 1501);
	int store_c = 0;
	char *p;
	p =strtok(buffer,"\n");
	store[store_c]=p;

	while(p = strtok(NULL,"\n")){
		store_c++;
	store[store_c]=p;
	}
		row *data;

	data = (row*) malloc (sizeof(row) * 5000);
	row tmpRow;
	tmpRow.rText = (char*) malloc (sizeof(char) * 1500);
	size_t curr_col =0;
	size_t atRow=0; 
	int rows = 0;
	while(rows<=store_c){
	strcpy(tmpRow.rText,store[rows]);	
	tmpRow.rowL =strlen(tmpRow.rText);
	tmpRow.rToken =(char**) malloc(sizeof(char *)* (28+1));
	tmpRow.rToken =tokenizer(tmpRow.rText,28);


			
		data[rows] = tmpRow;
				//printf("%s-----\n",data[x].rText);

	rows++;
	} 
	//printf("%d---%d\n",colnumber,rows);
	mergeSort(data, colnumber, rows);
	int k = 0;
	while(k<rows){
				printf("%s-----\n",data[k].rText);
	k++;
	}


}



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

	
	if (listen(s_sock, 300) < 0)
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
	pthread_t tid[500];
	char sendt[256];
	char* p = "water";	
	strcpy(sendt,p);
	//printf("%s",sendt);
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

	
	int dump = 0;
	int tmp= 0;
	read(c_sock,&tmp,sizeof(tmp));
	dump = ntohl(tmp);

	//printf("%d\n", dump);	

	if(dump == 0){
	pthread_create(&tid[i], NULL, sorter, &c_sock);
	}else{


	}


	i++;

	}

	

	

	/* clean up */
	close(s_sock);
	close(c_sock);
	return 0;




}


