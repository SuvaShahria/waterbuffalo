
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

#include <sys/wait.h>

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include "sorter_thread.h"

char* input;
char* odir;
char* hostname;
int port;

void	sorter(void* arg){
//needs changing since server sorts

}

void openDir(void* arg){

pthread_t tids[255];
pthread_t csvtids[255];

DIR *dir;
struct dirent *di;

 struct store* pas= (struct store*) arg;
	char* path = pas->dirname;

	 char** dirpath = (char**)malloc(sizeof(char*)*2000);
	struct store** directory = (struct store**)malloc(sizeof(struct store*)*256);


	char** csvpath = (char**)malloc(sizeof(char*)*2000);
    	struct store** csv = (struct store**)malloc(sizeof(struct store*)*256);


int  dcounter=0;
int csvcounter =0;
//fflush(stdout); // flush buffer for stdout

	if(!(dir = opendir(path))){ // check to see if path exists

printf("%s  path----------fail\n",path);
return;
}
//dir = opendir(path);

// go thru directory
while( (di =readdir(dir)) != NULL){
//ignore parent and current directory
	if(strcmp(di->d_name,".") != 0 && strcmp(di->d_name,"..") != 0){ //x
//printf("%s\n",di->d_name);
	// directory
	if(di->d_type==DT_DIR){//w
char* dpath= malloc(sizeof(char)*1500);
strcpy(dpath,path);
strcat(dpath,"/");
strcat(dpath,di->d_name);
 //printf("%s\n",dpath);
	

   dirpath[dcounter] = malloc(strlen(dpath)+1);
   dirpath[dcounter] = strcpy(dirpath[dcounter], dpath);
   directory[dcounter] = (struct store*)malloc( strlen(dpath) + 1);
   directory[dcounter] -> dirname = dpath;
	// printf("%s\n",directory[dcounter]->dirname);
            dcounter++;

 //printf("%s\n",path);
//path ="/home/user/Desktop/proj";
//dir = opendir(path);





}else{ //w
//not dir
	char* tmp2= di->d_name;
	int length = strlen(tmp2);
		if(tmp2[length-4]=='.' &&tmp2[length-3]=='c' && tmp2[length-2]=='s'  && tmp2[length-1]=='v'     ){//j
char* path2= malloc(sizeof(char)*1500);
strcpy(path2,path);
strcat(path2,"/");
strcat(path2,di->d_name);
if(1){ //check to see if it is right csv file validCsv(path2)
//printf("ayyy\n");


		 csvpath[csvcounter] = malloc(strlen(path2)+1);
				   csvpath[csvcounter] = strcpy(csvpath[csvcounter], path2);
				   csv[csvcounter] = (struct store*)malloc(strlen(path2) + 1);

				   csv[csvcounter] -> dirname = path2;
					// printf("%s\n",csv[csvcounter]->dirname);
				   csvcounter++;	
		

//sort here

}
//printf("----%s----\n",di->d_name);

} //j


}//w
}


}//end

	

	closedir(dir);

		int i =0;
	int test;
	for(i = 0; i < dcounter; i++){
		test = pthread_create(&tids[i], NULL, (void *)&openDir, (void*)directory[i]);
        if(test != 0){
            printf("Failed to create new thread.\n");
        }    
	}


	int r;
	int j;
	//ret here
	
	for(j = 0 ; j < csvcounter; j++){
		test = pthread_create(&csvtids[j], NULL, (void *)&sorter, (void*)csv[j]);

	
        if(test != 0){
            printf("Failed to create new thread.\n");
        }    
	}

	

for (j = 0; j < csvcounter; j++){
		pthread_join(csvtids[j], NULL);
	}
for (i = 0; i < dcounter; i++){
		pthread_join(tids[i], NULL);
	}


for (i = 0; i < dcounter; i++){
		free(directory[i]);
	}
for (i = 0; i < csvcounter; i++){
		free(csv[i]);
	}



	
//printf("%d\n",getpid());

  }


int main(int argc, char **argv){

	char cw[1024];
	if(getcwd(cw,sizeof(cw)) !=NULL){
	//printf("this dir is %s\n",cw);
	}


struct store* pass = (struct store*)malloc(sizeof(struct store* ));


if (argc < 3)
	{
		fprintf(stderr, "usage %s hostname portnum\n", argv[0]);
		exit(0);
	}

	// used later final=(row*)malloc(sizeof(row)*100);

	int sockfd;


input = (char*)malloc(sizeof(char*)*200);
odir = (char*)malloc(sizeof(char*)*200);	
 hostname = (char*)malloc(sizeof(char*)*200);;

	

	
	
	struct sockaddr_in address;
	// We need a hostent struct pointer to 
	// hold server info from gethostbyname()
	struct hostent *server;

	// We need a buffer for messages
	char buffer[256];
	
	int c=0,d=0,o=0,h=0,p=0;
	// =0 doesnt exist 1 = exist implement check later - ignore for testing
	// get arg
	int k =1;
	while(argv[k]){

	if(strcmp(argv[k],"-c")==0){
	input=strcpy(input,argv[k+1]);
	//printf("%s input\n",input);
	c=1;
	}
	
	
	if(strcmp(argv[k],"-p")==0){
	port = atoi(argv[k+1]);
	//printf("%d port\n",port);
	p=1;
	}
	
	if(strcmp(argv[k],"-h")==0){
	hostname=strcpy(hostname,argv[k+1]);
	//printf("%s hostname\n",hostname);
	h=1;	
	}

	if(strcmp(argv[k],"-d")==0){
	char* tmp = argv[k+1];
	pass->dirname=tmp;
	//printf("%s dirname\n",pass->dirname);
	d=1;
	}
	
	if(strcmp(argv[k],"-o")==0){
	odir=strcpy(odir,argv[k+1]);
	o = 1;
	//	printf("%s odir\n",odir);
	}	
	
	k++;
	k++;
	}  // end while loop
	
	// pass struct with which directory to start info
	openDir(pass);
	
	// the socket stuff doesnt go in main just here to test
	
	
	//this is how he did it in sakai i didnt use this
	

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

	address.sin_family = AF_INET;

	address.sin_port = htons(port);
	address.sin_addr.s_addr =inet_addr(hostname);
	
	// replace corresponding parts
	// we do binary copy to initialize sockaddr_in address part
	// from hostent pointer
	//sakai didnt use
	/*bcopy((char*)server->h_addr,
			(char*)&address.sin_addr.s_addr,
			server->h_length);
					*/

	//create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{

		perror("sock");

		exit(EXIT_FAILURE);
	}

	//connect socket to addr info created
	if (connect(sockfd, (struct sockaddr*)&address, sizeof(address)) < 0)
	{

		perror("connect");

		close(sockfd);

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

		perror("write");
	}

	// call read to receive data from server
	memset(buffer, 0, 256);
	n = read(sockfd, buffer, 256);
	if (n < 0)
	{
		// you know what to do
		perror("read");
	}

	printf("from server: %s", buffer); 
	

	close(sockfd);
	
	// this stuff is for creating the final csv
row header;
header.numc=28;
header.rToken=(char**)malloc(sizeof(char*)*500  );


header.rToken[0]="color";
header.rToken[1]="director_name";
header.rToken[2]="num_critic_for_reviews";
header.rToken[3]="duration";
header.rToken[4]="director_facebook_likes";
header.rToken[5]="actor_3_facebook_likes";
header.rToken[6]="actor_2_name";
header.rToken[7]="actor_1_facebook_likes";
header.rToken[8]="gross";
header.rToken[9]="genres";
header.rToken[10]="actor_1_name";
header.rToken[11]="movie_title";
header.rToken[12]="num_voted_users";
header.rToken[13]="cast_total_facebook_likes";
header.rToken[14]="actor_3_name";
header.rToken[15]="facenumber_in_poster";
header.rToken[16]="plot_keywords";
header.rToken[17]="movie_imdb_link";
header.rToken[18]="num_user_for_reviews";
header.rToken[19]="language";
header.rToken[20]="country";
header.rToken[21]="content_rating";
header.rToken[22]="budget";
header.rToken[23]="title_year";
header.rToken[24]="actor_2_facebook_likes";
header.rToken[25]="imdb_score";
header.rToken[26]="aspect_ratio";
header.rToken[27]="movie_facebook_likes";	

	


		
	return 0;
}
