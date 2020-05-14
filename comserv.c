


#include <stdio.h> 
#include <string.h> 
# include <stdlib.h>
# include <stdbool.h>
#include <ctype.h>
#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr


#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>


#include "protoserv.h"



int shmid, numtimes;
struct shmseg *shmp;
char *bufptr;
int spaceavailable;
int total_allowed_users;
int num_of_users;
char **name_of_users;
int *user_scores;
int board_array[5][5];
int num_of_ships , row, col;
int *cient_sock_arr;




int main(void)
{ 


	clear_screen();

        char * winner_name;
        admin_setup();
        
        printf("Waiting for : ");
        printf("%d" , total_allowed_users);
        printf(" users to enter into the game to start. \n");
        
        
        initialize_Variables();
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
        
        while(total_allowed_users > num_of_users){	

	printf("Listening for Incoming Connections.....\n");
         	memset(client_message,'\0',sizeof(client_message)); 
         	memset(username,'\0',sizeof(username));
         	memset(server_message,'\0',sizeof(server_message));    
	
	client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);

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
		
		if(total_allowed_users > num_of_users) {
			if (allow(username) == true){					
				strcpy(server_message, "1");
				if (send(client_sock, server_message, strlen(server_message),0)<0)
        				{
                				printf("Send Failed. Error!!!!!1\n");
                				return NULL;
        				}
        				SavingUser(username);
        				cient_sock_arr[num_of_users] = client_sock;     
        									
        				printf("Player Inserted : \n");
        				printf("%d\n" , cient_sock_arr[num_of_users]);
        				num_of_users++ ;
			}
			else{
				strcpy(server_message, "0");
				if (send(client_sock, server_message, strlen(server_message),0)<0)
        				{
                				printf("Send Failed. Error!!!!!1\n");
                				return NULL;
        				}
			
			}
		}
		else printf("Number of players in the room are at max.\n");
		//printf("%d" , num_of_users);
		//printf("%d" , total_allowed_users);
	}
	else printf("\nClient rejected : \n");
        }
        
        
        printf("Total number of allowed Players in the game entered.\n");
        
        
        int gameStatus = 1; //1 means game running;
        
        //////////////////
        int m=0;
        
        for (int client = cient_sock_arr[m]; m < total_allowed_users; client = cient_sock_arr[m]){
        	m++;
        	printf("Client number : " );
        	printf("%d\n " , client); 
        	
        	memset(server_message,'\0',sizeof(server_message));
	strcpy(server_message, "Get Ready for you Turn. The game has started!");

	if (send(client, server_message, strlen(server_message),0)<0)
	{
		printf("Send Failed. Error!!!!!1\n");
		return NULL;
	} 
        	
        	
        	    	
        }
        /////////////////////////
        
        clear_screen();
        printBoard();
        
        while (gameStatus == 1) {
        
        	printf("Managing Game\n");
		
	SharedMemory_Data(); //updating data in array of shared memory 
         		
         	saveDataToSharedMemory(); //sending data
        	
        	int i = 0;
  
        	for (int client = cient_sock_arr[i]; i < total_allowed_users; client = cient_sock_arr[i]){
        	
        		memset(client_message,'\0',sizeof(client_message));         	
         		memset(server_message,'\0',sizeof(server_message));

        	
        		//printf("\n%d " , client);
        		
        		strcpy(server_message, "Take Your Turn!");
		if (send(client, server_message, strlen(server_message),0)<0)
        		{
               		printf("Send Failed. Error!!!!!1\n");
                		return NULL;
        		}
        		
        		printf("Waiting to receive coordinates \n");
        		
        		if (recv(client, client_message, sizeof(client_message),0) < 0){
			printf("Receive Failed. Error!!!!!\n");
			return -1;
		}
		
		printf("client Message: \n%s\n",client_message);
		
		int row, col;
		coodinates(client_message, &row, &col);
		
		printf ("\n%d" , row);
		printf ("\n%d" , col);
		
		int attack_result = attack(row, col, i);
		
		clear_screen();
		
		printScoreBoard();

	
		printBoard();

		        	
	        	memset(server_message,'\0',sizeof(server_message));
	     	        	
	        	
	        	if(attack_result == 0) {
		
			strcpy(server_message, "You Miss");
		
			printf("Miss \n");
		
			//miss
		}
		else if (attack_result == 1) {
		
			strcpy(server_message, "You Hit");
		
			printf("Hit \n");
			//hit
		
		}
		else if (attack_result == 2) {
		
			strcpy(server_message, "You Sunk the Ship!");
		
			printf("Ship Sunk \n");
				
			//sunk
		}
        	
        		if (send(client, server_message, strlen(server_message),0)<0)
	        	{
	             		printf("Send Failed. Error!!!!!1\n");
		  	return NULL;
	        	}
        		

		gameStatus = isGameOver();
		
		if (gameStatus == 0 ) {
			winner_name = Checkwin();
			
			SharedMemory_Data(); //updating data in array of shared memory 
         			saveDataToSharedMemory(); //sending data
         			
			break;
			//send to the winner client		
		
		}
		i++;
	
        	}

        }
        
        int k = 0;
        for (int client = cient_sock_arr[k]; k < total_allowed_users; client = cient_sock_arr[k]){
        
        	k++;
        	     	
         	memset(server_message,'\0',sizeof(server_message));
        	
        	strcpy(server_message, winner_name);
	if (send(client, server_message, strlen(server_message),0)<0)
        	{
             		printf("Send Failed. Error!!!!!1\n");
                	return NULL;
        	}
        
        }
        
        freeResources();
        return 0;
}
