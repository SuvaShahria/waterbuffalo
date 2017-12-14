
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "sorter_thread.h"
long mainT;

int indx = 0;
int counter=0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex4 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex5 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex6 = PTHREAD_MUTEX_INITIALIZER;
char* input;
int arg3=0;
int finalcounter=0;
row* final;
char* input;
char* odir;
char* tmp;
int size=0;
int val=0;
int fx=0;




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


	

int tokens(FILE *fp,row* data,int numc){
	row tmpRow;
	tmpRow.rText = (char*) malloc (sizeof(char) * 1500);
	size_t curr_col =0;
	size_t atRow=0; 
	
	while(fgets(tmpRow.rText, 1500-1, fp) != NULL){
		tmpRow.rowL =strlen(tmpRow.rText);
		tmpRow.rToken =(char**) malloc(sizeof(char *)* (numc+1));
		tmpRow.rToken =tokenizer(tmpRow.rText,numc);


			
		data[atRow++] = tmpRow;		
	}
	

	return atRow;
}




void	sorter(void* arg){

pthread_mutex_lock(&mutex6);
 struct store* pas= (struct store*) arg;
char* path = pas->dirname;

pthread_mutex_lock(&mutex4);
if(mainT != pthread_self()){
	printf("%ld,",pthread_self());
}
char* colname = input;
pthread_mutex_unlock(&mutex4);


//printf("%s\n",name);
FILE *fp; 
fp = fopen(path,"r");
int j = 0;

	char *token;
	
	size_t numc = 1;
	row* datatest;
	row header;
	header.rText = (char*) malloc (sizeof(char) * 1500);		
	fgets(header.rText, 1500-1, fp);
	header.rowL = strlen(header.rText);
	header.rToken = (char**) malloc(sizeof(char *) * header.rowL);
	token = strtok(header.rText, ",");
	header.rToken[0] = token;

	while(token = strtok(NULL, ",")){
			
header.rToken[numc++] = token;	
	}
	header.numc = numc;


int length = strlen(header.rToken[numc - 1]);
	if(header.rToken[numc - 1][length - 1] == '\n'){
		header.rToken[numc - 1][length - 2] = '\0';
	}


//trim space
int i = 0;
//trim white space
	while(i < numc){

char *txt = header.rToken[i];
int l =	strlen(header.rToken[i])-1;
char *end;
while(isspace(*txt)){
txt++;
}
if(*txt==0){
header.rToken[i] =txt;
}else{
end = txt +l;
while(end>txt && isspace(*end)){
end--;
}
	*(end+1)=0;
header.rToken[i] =txt;
}



	//printf("====%s====\n", header.rToken[i]);
		i++;
	}

	row *data;
	int rows; 
	data = (row*) malloc (sizeof(row) * 5000);
	rows = tokens(fp, data, numc);


	
int x =0;

	while(x < header.numc){
		if(strcmp(header.rToken[x], colname) == 0){
			break;
		}
		x++;
	}
	fx=x;
		printf("%s---\n",data[rows-1].rText);
mergeSort(data, x, rows);
	char* w = (char*)malloc(sizeof(char)* 2000 );
		printf("%s---\n",data[rows-1].rText);
	w= data[0].rText;
//	printf("\n%s, %ld",w,strlen(data[0].rText));

	pthread_mutex_lock(&mutex5);
	val=x;
	 size = size+rows +1;
	row* d2;
	d2=(row*)realloc (final,sizeof(row)*(size*+100) );
	final = d2;
	int q = 0;
	for(q=0;q<rows;q++){
	final[finalcounter]=data[q];
	finalcounter++;
	}	
	if(final[0].rToken[1]){

	}
	//printf("\n%s",data[0].rToken[1]);
		if(mainT != pthread_self()){
	//pthread_exit(0);	
//printf("%ld,",pthread_self());
}
	pthread_mutex_unlock(&mutex5);
	
pthread_mutex_unlock(&mutex6);

	
//free(data)
}//end



//void tokens(FILE *fp,row* data,int numc);








int validCsv(char* path){
FILE *fp; 
fp = fopen(path,"r");
int j = 0;
	  
	char *token;
	size_t numc = 1;
row header;
	header.rText = (char*) malloc (sizeof(char) * 1500);		
	fgets(header.rText, 1500-1, fp);
	header.rowL = strlen(header.rText);
	header.rToken = (char**) malloc(sizeof(char *) * header.rowL);
	token = strtok(header.rText, ",");
	header.rToken[0] = token;

	while(token = strtok(NULL, ",")){
			
header.rToken[numc++] = token;	
	}
	header.numc = numc;


int length = strlen(header.rToken[numc - 1]);
	if(header.rToken[numc - 1][length - 1] == '\n'){
		header.rToken[numc - 1][length - 2] = '\0';
	}


//trim space
int i = 0;
//trim white space
	while(i < numc){

char *txt = header.rToken[i];
int l =	strlen(header.rToken[i])-1;
char *end;
while(isspace(*txt)){
txt++;
}
if(*txt==0){
header.rToken[i] =txt;
}else{
end = txt +l;
while(end>txt && isspace(*end)){
end--;
}
	*(end+1)=0;
header.rToken[i] =txt;
}



//	printf("====%s====\n", header.rToken[i]);
		i++;
	}


if(!strcmp(header.rToken[0],"color") &&!strcmp(header.rToken[1],"director_name")&&!strcmp(header.rToken[2],"num_critic_for_reviews")&&!strcmp(header.rToken[4],"director_facebook_likes") &&!strcmp(header.rToken[5],"actor_3_facebook_likes")&&!strcmp(header.rToken[6],"actor_2_name")&&!strcmp(header.rToken[7],"actor_1_facebook_likes")&&!strcmp(header.rToken[8],"gross") &&!strcmp(header.rToken[9],"genres") &&!strcmp(header.rToken[10],"actor_1_name") &&!strcmp(header.rToken[11],"movie_title") &&!strcmp(header.rToken[12],"num_voted_users") &&!strcmp(header.rToken[13],"cast_total_facebook_likes") &&!strcmp(header.rToken[14],"actor_3_name")&&!strcmp(header.rToken[15],"facenumber_in_poster") &&!strcmp(header.rToken[16],"plot_keywords") &&!strcmp(header.rToken[17],"movie_imdb_link") &&!strcmp(header.rToken[18],"num_user_for_reviews") &&!strcmp(header.rToken[19],"language") &&!strcmp(header.rToken[20],"country") &&!strcmp(header.rToken[21],"content_rating") &&!strcmp(header.rToken[22],"budget") &&!strcmp(header.rToken[23],"title_year") &&!strcmp(header.rToken[24],"actor_2_facebook_likes") &&!strcmp(header.rToken[25],"imdb_score") &&!strcmp(header.rToken[26],"aspect_ratio") &&!strcmp(header.rToken[27],"movie_facebook_likes") &&!strcmp(header.rToken[3],"duration") ){//lkj
return 1;
}
return 0;
}




void test(void* arg){

 struct store* pas= (struct store*) arg;
	char* path = pas->dirname;
printf("%s --test\n",pas->dirname);
}

void openDir(void* arg){
	pthread_mutex_lock(&mutex3);
if(mainT != pthread_self()){
	printf("%ld,",pthread_self());
}
	pthread_mutex_unlock(&mutex3);

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
	pthread_mutex_lock(&mutex2);
counter++;
	pthread_mutex_unlock(&mutex2);

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
if(validCsv(path2)){ //check to see if it is right csv file validCsv(path2)
//printf("ayyy\n");
pthread_mutex_lock(&mutex2);
counter++;
	pthread_mutex_unlock(&mutex2);

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










int main (int argc, char* argv[]){
printf("Initial TID: %ld\n",pthread_self());

char* dirname;

mainT =pthread_self();


char cw[1024];
if(getcwd(cw,sizeof(cw)) !=NULL){
//printf("this dir is %s\n",cw);
}


struct store* pass = (struct store*)malloc(sizeof(struct store* ));

//printf("%s--\n",pass->dirname);
//openDir(pass);

final=(row*)malloc(sizeof(row)*100);
int c=0,d=0,o=0;
int x =1;
input = (char*)malloc(sizeof(char*)*200);
odir = (char*)malloc(sizeof(char*)*200);

if(argc-1==2){//2
//printf("%d 2\n",argc);
while(argv[x]){


if(strcmp(argv[x],"-c")==0){
input=strcpy(input,argv[x+1]);
//printf("%s input\n",input);
c=1;
}



x++;
x++;
}//while


}else if(argc-1==4){//4
//printf("%d 4\n",argc);
while(argv[x]){


if(strcmp(argv[x],"-c")==0){
input=strcpy(input,argv[x+1]);
//printf("%s input\n",input);
c=1;
}


if(strcmp(argv[x],"-d")==0){

char* tmp = argv[x+1];
pass->dirname=tmp;
//printf("%s dirname\n",pass->dirname);
d=1;
}


if(strcmp(argv[x],"-o")==0){
odir=strcpy(odir,argv[x+1]);
//printf("%s odir\n",odir);
o=1;
}


x++;
x++;
}//while






}else if(argc-1==6){ //6

//printf("%d 6\n",argc);


while(argv[x]){


if(strcmp(argv[x],"-c")==0){
input=strcpy(input,argv[x+1]);
//printf("%s input\n",input);
c=1;
}


if(strcmp(argv[x],"-d")==0){

char* tmp = argv[x+1];
pass->dirname=tmp;
//printf("%s dirname\n",pass->dirname);
d=1;
}


if(strcmp(argv[x],"-o")==0){
odir=strcpy(odir,argv[x+1]);
//printf("%s odir\n",odir);
o=1;
}


x++;
x++;
}//while



}




if(c==0){
printf("-c entry was not given ----failed\n");
return 0;
}


if(d==0){
pass->dirname=cw;
}


if(o==0){
odir=cw;

}


/*printf("%s\n",input);
printf("%s\n",pass->dirname);
printf("%s\n",odir);*/

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

DIR *dir2;

	if(!(dir2 = opendir(odir))){ // check to see if path exists

printf("%s  output directory failed, check to see if entered correctly----------fail\n",odir);
return 0;
}
closedir(dir2);



printf("TIDS of all child threads: ");


counter++;

//ret here




if(strcmp(input,"color")==0 ||strcmp(input,"director_name")==0||strcmp(input,"num_critic_for_reviews")==0 ||strcmp(input,"director_facebook_likes")==0 ||strcmp(input,"actor_3_facebook_likes")==0 ||strcmp(input,"actor_2_name")==0 ||strcmp(input,"actor_1_facebook_likes")==0 ||strcmp(input,"gross")==0 ||strcmp(input,"genres")==0 ||strcmp(input,"actor_1_name")==0 ||strcmp(input,"movie_title")==0 ||strcmp(input,"num_voted_users")==0 ||strcmp(input,"cast_total_facebook_likes")==0 ||strcmp(input,"actor_3_name")==0 ||strcmp(input,"facenumber_in_poster")==0 ||strcmp(input,"plot_keywords")==0 ||strcmp(input,"movie_imdb_link")==0 ||strcmp(input,"num_user_for_reviews")==0 ||strcmp(input,"language")==0 ||strcmp(input,"country")==0 ||strcmp(input,"content_rating")==0 ||strcmp(input,"budget")==0 ||strcmp(input,"title_year")==0 ||strcmp(input,"actor_2_facebook_likes")==0 ||strcmp(input,"imdb_score")==0 ||strcmp(input,"aspect_ratio")==0 ||strcmp(input,"movie_facebook_likes")==0 ||strcmp(input,"duration")==0 ){
openDir(pass);
//openDir(&pass);
}else{

printf("invalid -c argument\n");
}

printf("\nTotal number of threads: %d\n",counter);

mergeSort(final,fx,finalcounter);
char* tp1 = "AllFiles";
char* tp2 ="-sorted-";
char* fil="/";
char* tp3 ="<";
char* tp4 =">";
char* tp5 =".csv";
char* csvfile=(char*)malloc(strlen(odir)+strlen(input)+20);
	strcpy(csvfile,odir);
		strcat(csvfile,fil);
	strcat(csvfile,tp1);
	strcat(csvfile,tp2);
	strcat(csvfile,tp3);
		strcat(csvfile,input);
		strcat(csvfile,tp4);
		strcat(csvfile,tp5);
		//printf("%s\n",csvfile);

//char* tmp ="/home/suva/Desktop/quack.csv";
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

i = 0;
	 j = 0;
	while(i < finalcounter){
		while(j < 28){



				
			fprintf(fp,"%s",final[i].rToken[j]);
			//printf("\n%i %i",i,j);
			if(j != 27){
				fprintf(fp,",");
			}else{
				fprintf(fp,"\n");
			}
			j++;
		}
		i++;
		j = 0;
	}




fclose(fp);


}else{
printf("final csv store but testor writing\n");

}

//fprintf(fp,"i love cheese\n" );

	


	


return 0;
}
