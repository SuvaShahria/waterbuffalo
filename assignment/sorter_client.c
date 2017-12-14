
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
#include "sorter_client.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

char* input;
char* odir;
char* hostname;
int port;
int colnumber;


void	sorter(void* arg){
//needs changing since server sorts
 struct store* pas= (struct store*) arg;
char* path = pas->dirname;

FILE *fp; 
fp = fopen(path,"r");

int sockfd;
	struct sockaddr_in address;
	struct hostent *server;
	//char buffer[256];

	//lock globals
	pthread_mutex_lock(&mutex);

	server = gethostbyname(hostname);

	if (server == NULL){
		fprintf(stderr, "ERROR, NO SUCH HOST\n");
		exit(0);
	}

	memset(&address, 0, sizeof(address));

	address.sin_family = AF_INET;

	address.sin_port = htons(port);
	address.sin_addr.s_addr =inet_addr(hostname);
	int coln = colnumber;
	//printf("%d colnumber\n",coln);
	int tmp2 = htonl(coln);
	pthread_mutex_unlock(&mutex);

	bcopy((char*)server->h_addr,
			(char*)&address.sin_addr.s_addr,
			server->h_length);
				
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0){
		perror("sock");
		exit(EXIT_FAILURE);
	}


	if (connect(sockfd, (struct sockaddr*)&address, sizeof(address)) < 0){

		perror("connect");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	fseek(fp,0,SEEK_END);//goes to end of csv	
	long bsize = ftell(fp)-419; //blinker position in csv
	fseek(fp,419,SEEK_SET); //goes back to beg
	char * buffer = malloc(sizeof(char)* bsize);
	fread(buffer, bsize, sizeof(char), fp); //write csv to buffer
	int l = 1;
	
	int dump = 0;
	int tmp = htonl(dump); // googled how to pass int
	long tmp3 = htonl(bsize);	
	//printf("bsize %ld\n",bsize);

	write(sockfd,&tmp,sizeof(dump) ); // to dump or not
	write(sockfd,&tmp2,sizeof(coln) ); // which coln
	write(sockfd,&tmp3,sizeof(bsize) ); 
	write(sockfd,buffer,bsize ); 
		
	//printf("%s\n",buffer);
	
	

//	printf("%s",buffer);
/*	printf("Please enter your message: ");

	fgets(buffer, 256, stdin);

	int n = write(sockfd, buffer, 256);

	
	if (n <= 0){

		perror("write");
	}


	memset(buffer, 0, 256);
	n = read(sockfd, buffer, 256);
	if (n < 0){

		perror("read");
	}

	printf("from server: %s", buffer); 
*/	

	close(sockfd);

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





odir = (char*)malloc(sizeof(char*)*300);	
 hostname = (char*)malloc(sizeof(char*)*20);;

	

	
	
	int c=0,d=0,o=0,h=0,p=0;
	// =0 doesnt exist 1 = exist implement check later - ignore for testing
	// get arg
	int k =1;
	while(argv[k]){

	if(strcmp(argv[k],"-c")==0){
	input = (char*)malloc(sizeof(char*)*sizeof(argv[k+1]));
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
	
if(c==0 || h == 0 || p == 0){
printf("c,h,p paramaters are required\n");
return 0;

}


if(d==0 ){
	pass->dirname=cw;

}

if(o==0 ){
	odir=cw;

}


 if(strcmp(input,  "color")==0) colnumber=0;
  else if(strcmp(input, "director_name")==0) colnumber=1;
  else if(strcmp(input, "num_critic_for_reviews")==0) colnumber=2;
  else if(strcmp(input, "duration")==0) colnumber=3;
  else if(strcmp(input, "director_facebook_likes")==0) colnumber=4;
  else if(strcmp(input, "actor_3_facebook_likes")==0) colnumber=5;
  else if(strcmp(input, "actor_2_name")==0) colnumber=6;
  else if(strcmp(input, "actor_1_facebook_likes")==0) colnumber=7;
  else if(strcmp(input, "gross")==0) colnumber=8;
  else if(strcmp(input, "genres")==0) colnumber=9;
  else if(strcmp(input, "actor_1_name")==0) colnumber=10;
  else if(strcmp(input, "movie_title")==0) colnumber=11;
  else if(strcmp(input, "num_voted_users")==0) colnumber=12;
  else if(strcmp(input, "cast_total_facebook_likes")==0) colnumber=13;
  else if(strcmp(input, "actor_3_name")==0) colnumber=14;
  else if(strcmp(input, "facenumber_in_poster")==0) colnumber=15;
  else if(strcmp(input, "plot_keywords")==0) colnumber=16;
  else if(strcmp(input, "movie_imdb_link")==0) colnumber=17;
  else if(strcmp(input, "num_user_for_reviews")==0) colnumber=18;
  else if(strcmp(input, "language")==0) colnumber=19;
  else if(strcmp(input, "country")==0) colnumber=20;
  else if(strcmp(input, "content_rating")==0) colnumber=21;
  else if(strcmp(input, "budget")==0) colnumber=22;
  else if(strcmp(input, "title_year")==0) colnumber=23;
  else if(strcmp(input, "actor_2_facebook_likes")==0) colnumber=24;
  else if(strcmp(input, "imdb_score")==0) colnumber=25;
  else if(strcmp(input, "aspect_ratio")==0) colnumber=26;
  else if(strcmp(input, "movie_facebook_likes")==0) colnumber=27;
  else{
	printf("invalid c argument- exiting\n");
    return 0;
}	
	//start

		pthread_t tid1;
		pthread_create(&tid1, NULL, (void *)openDir, (void*)pass);
				pthread_join(tid1, NULL);
	

		
	int sockfd;

	
	struct sockaddr_in address;
	
	struct hostent *server;

	
	server = gethostbyname(hostname);

	if (server == NULL)
	{

		fprintf(stderr, "ERROR, NO SUCH HOST\n");
		exit(0);
	}
	
	memset(&address, 0, sizeof(address));

	address.sin_family = AF_INET;

	address.sin_port = htons(port);
	
	bcopy((char*)server->h_addr,
			(char*)&address.sin_addr.s_addr,
			server->h_length);
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		//perror() report error message
		perror("sock");
		//exit your program
		exit(EXIT_FAILURE);
	}

	
	if (connect(sockfd, (struct sockaddr*)&address, sizeof(address)) < 0)
	{
		// perror() report error message
		perror("connect");
		// close socket
		close(sockfd);
		// exit your program
		exit(EXIT_FAILURE);
	}
	int dump = 1;
	int tmp = htonl(dump); // googled how to pass int

	//printf("bsize %ld\n",bsize);

	write(sockfd,&tmp,sizeof(dump) ); 
	long bl = 0;	
	long tmpbl =0;
	read(sockfd,&tmpbl,sizeof(tmpbl));
	 bl = ntohl(tmpbl);
	printf("%ld\n",bl);
//	printf("%d bl\n",bl);
	
	char buf[bl+1];
	bzero(buf,bl+1);
	read(sockfd,buf,bl);
	//printf("%s\n",buf);
	buf[bl-1]='\0';
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

char* tp1 = "AllFiles";
char* tp2 ="-sorted-";
char* fil="/";
char* tp3 ="<";
char* tp4 =">";
char* tp5 =".csv";
char* csvfile=(char*)malloc(strlen(odir)+strlen(input)+40);

	strcpy(csvfile,odir);
		strcat(csvfile,fil);
	strcat(csvfile,tp1);
	strcat(csvfile,tp2);
	strcat(csvfile,tp3);
		strcat(csvfile,input);
		strcat(csvfile,tp4);
		strcat(csvfile,tp5);


	
FILE *fp;
if(fp=fopen(csvfile,"w")){
int i;
i = 0;
	//print 
	while(i < 28){
		fprintf(fp,"%s",header.rToken[i]);
		if(i != 27){
			fprintf(fp,",");
		}else{
			fprintf(fp,"\n");
		}
		i++;
	}

int j;
fprintf(fp,"%s",buf);

fclose(fp);


}else{
printf("final csv store but testor writing\n");

}


		
	return 0;
}
