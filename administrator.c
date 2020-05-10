
#include <stdio.h> 
#include <string.h> 
# include <stdlib.h>
# include <stdbool.h>
#include <ctype.h>
#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr



int total_allowed_users;
int num_of_users=0;
char **name_of_users;
int *user_scores;
int board_array[5][5];


void clear_screen(){
    system("clear");
}

void updateScore(int user_index,int score){
	user_scores[user_index]=user_scores[user_index]+score;
}

int attack(int row, int column,int user_index){
// return 0 means "miss" | return 1 means "hit" | return 2 means "sunk"

	if(board_array[row-1][column-1]!=0){
		board_array[row-1][column-1]=board_array[row-1][column-1]-1;
		updateScore(user_index,10);
		if(board_array[row-1][column-1]==0){
			return 2;
		}
		return 1;
	}
	else{
	return 0; 
	}
}

void printScoreBoard(){



    printf("===============\n|  ");
    printf("Score Board: \n");
    for(int i = 0; i < num_of_users; ++i)
    {
	printf("|  ");
	printf("%s",name_of_users[i]);
	printf(": ");
        printf("%d\t", user_scores[i]);
	printf("\n");
    }
    printf("===============\n");

  
}


void printBoard(){

	printf("--------------------------------------------------------------------------------\n");
	printf("\n");
	for(int i=0;i<5;i++){
		printf("|\t");
		for(int j=0;j<5;j++){
			printf("%d\t", board_array[i][j]);
			printf("|\t");
		}
		printf("\n");
		printf("\n");
	}
	printf("--------------------------------------------------------------------------------\n");
}

void freeResources(){
	//////////////////////////
	for ( int i = 0; i < total_allowed_users; i++ )
	{
	    free(name_of_users[i]);
	}

	free(name_of_users);
	//////////////////////////
	free(user_scores);
}

void initialize_Variables(){

	name_of_users = (char*) calloc(total_allowed_users, sizeof(char));

	for ( int i = 0; i < total_allowed_users; i++ )
	{
	    name_of_users[i] = (char*) calloc(100, sizeof(char)); // 100 represents the size
	}
	////////////////////////////
	user_scores = (int*) malloc(num_of_users * sizeof(int));
	 
	// if memory cannot be allocated
	if(user_scores == NULL)                     
	{
		printf("Error! memory not allocated.");
		exit(0);
	}
}

bool allow(char * username){
    
    char allow[1];
    
    printf("\nDo you want to allow user to enter in the game room. \n");
    printf("User name is : ");
    printf("%s\n" , username);
    printf("Please Select an option [y/n] : ");
    gets(allow);
    
    if (allow[0] == 'y') {
    	printf("User allowed to enter the Game Room. \n");
    	return true;
    }
    printf("User is not allowed to Enter the Game Room. \n");
    return false;

}

bool isUser(char * UserIncoming){

    FILE *fp;
    int MAXCHAR = 100;
    char str[MAXCHAR];
    char* filename = "users.txt";
 
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s",filename);
        return 1;
    }
    bool is_user = false;
    while (fgets(str, MAXCHAR, fp) != NULL) {
        
        int string_size = strlen(str);
        int user_size = strlen(UserIncoming);
 
        
        if(string_size == user_size) {
        	
        	is_user = false;       	
        	for(int i=0; i<user_size; i++){
        	    
        	     if(UserIncoming[i] == str[i]) {
        	     	is_user = true;
        	     }
        	     else {
        	     	is_user = false;
        	     	break;
        	     }
        	
        	}
        	if(is_user == true){
        	     
        	     fclose(fp);
        	     return is_user;	
        	}
        }
    }
    
    fclose(fp);
    return is_user;
}
    

int main(void)
{ 

        initialize_Variables();
        printf("Please allow total users you would like to allow :  ");
        scanf("%d", &total_allowed_users); 
        
        printf("Waiting for : ");
        printf("%d" , total_allowed_users);
        printf(" users to enter into the game to start. \n");
        
        
 
        int socket_desc, client_sock, client_size; 
        struct sockaddr_in server_addr, client_addr;         
	//SERVER ADDR will have all the server address
        char server_message[2000], client_message[2000];
        char username[100];       
	// Sending values from the server and receive 
	//from the server we need this
        //Cleaning the Buffers
        
        memset(username,'\0',sizeof(username));
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));     
	// Set all bits of the padding field//        
        //Creating Socket
        
        
        
        socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	
	        
        if(socket_desc < 0)
        {
                printf("Could Not Create Socket. Error!!!!!\n");
                return -1;
        }
        
        printf("Socket Created\n");
        
        //Binding IP and Port to socket
        
        server_addr.sin_family = AF_INET;               
	/* Address family = Internet */
        server_addr.sin_port = htons(2001);              
	// Set port number, using htons function to use proper byte order */
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");    
	/* Set IP address to localhost */


	// BINDING FUNCTION
        
        if(bind(socket_desc, (struct sockaddr*)&
	server_addr, 	sizeof(server_addr))<0) { 
	// Bind the address struct to the socket.  /
	//bind() passes file descriptor, the address structure,
	//and the length of the address structure
        
                printf("Bind Failed. Error!!!!!\n");
                return -1;
        }        
        
        printf("Bind Done\n");
        
        //Put the socket into Listening State

	 
        if(listen(socket_desc, 1) < 0) {                            
	 // This listen() call tells the socket to listen 
	 // to the incoming connections.
	 // The listen() function places all incoming 
	 // connection into a "backlog queue" until accept()
	 // call accepts the connection.
         
                printf("Listening Failed. Error!!!!!\n");
                return -1;
         }
        
        
        //Accept the incoming Connections
        
        client_size = sizeof(client_addr);
        
        while(1){	

	printf("Listening for Incoming Connections.....\n");
         	memset(client_message,'\0',sizeof(client_message)); 
         	memset(username,'\0',sizeof(username));
         	memset(server_message,'\0',sizeof(server_message));    
	
	client_sock = accept(socket_desc, 
	(struct sockaddr*)&client_addr, &client_size);

	if (client_sock < 0){
		printf("Accept Failed. Error!!!!!!\n");
		return -1;
	}
	printf("Client Connected with IP: %s and Port No: %i\n", 		
	inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
	//inet_ntoa() function converts the Internet  			   
	//host address in, given in network byte order, 
	//to a string in IPv4 dotted-decimal notation

	if (recv(client_sock, client_message, sizeof(client_message),0) < 0){
		printf("Receive Failed. Error!!!!!\n");
		return -1;
	}

	//printf("Client Message: %s\n",client_message);
	if ( isUser(client_message) == true) {
		
		for (int i=0; client_message[i] != '@' ; i++){
			username[i] = client_message[i];	
		}
		printf("Player Validated : \n");
		allow(username);
	}
	else printf("\nClient rejected : \n");
        }
        
        freeResources();
        return 0;
}
        
