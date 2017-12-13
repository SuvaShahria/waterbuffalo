#include "sorter_thread.h"

#define PORT 9000
#define ADDR 127.0.0.1

char* input;
char* odir;
char* hostname;
int port;

int main(int argc, char *argv[]) {

	//check minimum number of arguements
	if (argc < 7) {
		printf("Not enough arguements\n");
		return 0;
	}

	// not sure what this is for tbh
	char cw[1024];
	if (getcwd(cw, sizeof(cw)) != NULL){

	}

	struct store* pass = (struct store*) malloc(sizeof(struct store*));
	input = (char*) malloc(sizeof(char*) * 200);
	odir = (char*) malloc(sizeof(char*) * 200);	
 	hostname = (char*) malloc(sizeof(char*) * 200);

	int sockfd;
	struct sockaddr_in address;
	struct hostent *server;		// We need a hostent struct pointer to hold server info from gethostbyname()
	char buffer[256];			// We need a buffer for messages
	
	// get arguements from call
	int c = 0, p = 0, h = 0, d = 0, o = 0;
	int k = 1;
	while (argv[k]) {

		if (strcmp(argv[k], "-c") == 0) {
			input = strcpy(input, argv[k+1]);
			c = 1;
		}
				
		if (strcmp(argv[k], "-p") == 0) {
			port = atoi(argv[k+1]);
			p = 1;
		}
		
		if (strcmp(argv[k], "-h") == 0) {
			hostname = strcpy(hostname, argv[k+1]);
			h = 1;	
		}

		if (strcmp(argv[k], "-d") == 0) {
			char* tmp = argv[k+1];
			pass->dirname = strcpy(pass->dirname, tmp);
			d = 1;
		}
		
		if (strcmp(argv[k], "-o") == 0) {
			odir = strcpy(odir, argv[k+1]);
			o = 1;
		}	
		
		k++;
		k++;
	}

	// missing required inputs
	if (c == 0 || p == 0 || h == 0) {
		printf("Missing required inputs\n");
		return 0;
	}
	
	if (d == 0) {
		getcwd(pass->dirname, sizeof(pass->dirname));
	}

	if (o == 0) {
		getcwd(odir, sizeof(odir));
	}

	// first we send the server the column to sort on
	sendSortCol();

	// pass struct with which directory to start info
	openDir(pass);
	
	// the socket stuff doesnt go in main just here to test
	//this is how he did it in sakai i didnt use this
	

	/*
	server = gethostbyname(argv[1]);
	// replace hostent pointer with your pointer name
	if (server == NULL) {
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
	header.numc = 28;
	header.rToken = (char**) malloc(sizeof(char*) * 500);
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

/*
Function to send the sort column.
*/
void sendSortCol() {
	int sock = 0;
	struct sockaddr_in address;
	struct sockaddr_in serv_addr;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Error creating socket\n");
		return;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		printf("Failed to connect\n");
			return;
		}

	char *sendingCol = "SortColumn";
	send(sock, sendingCol, strlen(sendingCol), 0);
	send(sock, input, strlen(input), 0);
	close(sock);
}

/*
Function to be called when a new CSV is found.
Establishes connection to server, then sends CSV.

params:	
return:	void
*/
void sorter(void* arg) {
	pthread_mutex_lock(&mutex6);
	struct store* pas = (struct store*) arg;
	char* path = pas->dirname;

	// create socket + establish a connection here

	FILE *fp; 
	fp = fopen(path, "r");
	// send the file in continuous packs here

	pthread_mutex_unlock(&mutex6);
}

/*
Function to start going through the given directory.
Stores all the subdirectories and appropriate CSV files into
arrays first, then calls a new thread for each of them.
Finally joins each thread and frees them at the end.

params:	
return: void
*/
void openDir(void* arg) {
	pthread_t tids[255];
	pthread_t csvtids[255];
	
	struct store* pas = (struct store*) arg;
	char* path = pas->dirname;

	char** dirpath = (char**) malloc(sizeof(char*) * 2000);
	struct store** directory = (struct store**) malloc(sizeof(struct store*) * 256);
	int  dcounter=0;

	char** csvpath = (char**) malloc(sizeof(char*) * 2000);
	struct store** csv = (struct store**) malloc(sizeof(struct store*) * 256);
	int csvcounter =0;

	// check to see if path exists
	DIR *dir;
	if (!(dir = opendir(path))) { 
		printf("%s  path----------fail\n",path);
		return;
	}

	// go thru directory
	struct dirent *di;
	while ((di = readdir(dir)) != NULL) {

		// ignoring the directions "." and ".."
		if (strcmp(di->d_name, ".") != 0 && strcmp(di->d_name, "..") != 0) { 

			// current dirent is a subdirectory
			if (di->d_type == DT_DIR) {
				char* dpath= malloc(sizeof(char) * 1500);
				strcpy(dpath, path);
				strcat(dpath, "/");
				strcat(dpath, di->d_name);

				dirpath[dcounter] = malloc(strlen(dpath) + 1);
				dirpath[dcounter] = strcpy(dirpath[dcounter], dpath);
				directory[dcounter] = (struct store*) malloc(strlen(dpath) + 1);
				directory[dcounter]->dirname = dpath;
				dcounter++;
			}

			// current dirent is not a subdirectory
			else { 
				char* tmp2 = di->d_name;
				int length = strlen(tmp2);
				
				// current dirent is a CSV
				if (tmp2[length-4] == '.' && tmp2[length-3] == 'c' && tmp2[length-2] == 's' && tmp2[length-1] == 'v') {
					char* path2= malloc(sizeof(char) * 1500);
					strcpy(path2, path);
					strcat(path2, "/");
					strcat(path2, di->d_name);
					
					//check to see if it is right csv file validCsv(path2)
					if (1) {
						csvpath[csvcounter] = malloc(strlen(path2) + 1);
						csvpath[csvcounter] = strcpy(csvpath[csvcounter], path2);
						csv[csvcounter] = (struct store*) malloc(strlen(path2) + 1);
						csv[csvcounter]->dirname = path2;
						csvcounter++;
					}
				}
			}
		}
	}

	closedir(dir);

	int i;
	int test;
	for (i = 0; i < dcounter; i++) {
		test = pthread_create(&tids[i], NULL, (void *) &openDir, (void*) directory[i]);
		if (test != 0) {
			printf("Failed to create new thread.\n");
		}
	}

	int j;
	for (j = 0; j < csvcounter; j++) {
		test = pthread_create(&csvtids[j], NULL, (void *) &sorter, (void*) csv[j]);	
		if (test != 0) {
			printf("Failed to create new thread.\n");
		}
	}

	for (j = 0; j < csvcounter; j++) {
		pthread_join(csvtids[j], NULL);
	}
	for (i = 0; i < dcounter; i++) {
		pthread_join(tids[i], NULL);
	}

	for (i = 0; i < dcounter; i++) {
		free(directory[i]);
	}
	for (i = 0; i < csvcounter; i++) {
		free(csv[i]);
	}
}