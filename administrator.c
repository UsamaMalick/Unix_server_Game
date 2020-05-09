/*
        TCP_Server. This Program will will create the Server side for TCP_Socket Programming.
        It will receive the data from the client and then send the same data back to client.
*/

#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr
#include <pthread.h>

int clients = 0;
//number of canidates allowed
char* canidates[10];
int count = 0;
void load_canidates()
{
	FILE *fin;  
	fin = fopen("Candidates_List.txt","r");
	char voter[100];
	int flag = 1;
	while(!feof(fin))
	{
		fgets(voter, sizeof(voter), fin);

		char *newline;
		if ((newline=strchr(voter, '\n')) != NULL)
		    *newline = '\0';

		char *carriage;
		if((carriage=strchr(voter, '\r')) != NULL)
			*carriage = '\0';
		
		canidates[count] = (char*) malloc(strlen(voter) * sizeof(char));
		flag = 1;
		for(int i=0, j=0; i<strlen(voter); i++)
		{
			//printf("%d: %c\n",voter[i], voter[i]);
			if(flag == 0)
				canidates[count][j++] = voter[i];
			if(voter[i] == 9)
				flag = 0;
		}
		count++;
	}
	fclose(fin);
}

void poll_msg(char server_message[], char Name[])
{
	strcpy(server_message, "Welcome ");
	strcat(server_message, Name);
	strcat(server_message, "\nChoose One symbol.\n");
	for(int k=0; k<count; k++)
	{
		char a[2];
		a[0] = k + 49;
		a[1] = '\0';
		strcat(server_message, a);
		strcat(server_message, ". ");
		strcat(server_message, canidates[k]);
		strcat(server_message, "\n");
	}
	//printf("%s\n", server_message);
	//return server_message;
}

void save_vote(char name[], char symbol[])
{
	FILE *fout;
	fout = fopen("output.txt", "a");
	fputs(name, fout);
	fputs("\t", fout);
	fputs(symbol, fout);
	fputs("\n", fout);
}
//flag 1 means exist
// flag 0 means not exist

int check_voter(char auth[])
{
	FILE *fin;  
	fin = fopen("Voters_List.txt","r");
	char voter[1000];

	printf("%s\n", auth);

	int flag = 0;
	while(!feof(fin))
	{
		fgets(voter, sizeof(voter), fin);

		char *newline;
		if ((newline=strchr(voter, '\n')) != NULL)
		    *newline = '\0';

		char *carriage;
		if((carriage=strchr(voter, '\r')) != NULL)
			*carriage = '\0';

		//printf("%s\n",voter);
		
		if(strcmp(voter, auth) == 0)	
		{
			flag = 1;
		}
	}
	fclose(fin);
	return flag;
}

void* client_req(void *des)
{
	char server_message[2000], client_message[2000];
	int client_sock = (int*)des;
	//int client_sock = *client_sock1;
	int flag = 1;
	char Name[100];
	
	printf("Client thread number: %d  %d\n", clients, client_sock);
	
	//Receive the message from the client
	if (recv(client_sock, client_message, sizeof(client_message),0) < 0)
	{
	    printf("Receive Failed. Error!!!!!\n");
	    return NULL;
	}
	printf("%s\n", client_message);
	
	strcpy(server_message, "Connected.\nEnter authentications(format Name/CNIC).");
	if (send(client_sock, server_message, strlen(server_message),0)<0)
	{
		printf("Send Failed. Error!!!!!\n");
		return NULL;
	}
	
	//authentications
	memset(client_message,'\0',sizeof(client_message));
	//Receive the message from the client
	if (recv(client_sock, client_message, sizeof(client_message),0) < 0)
	{
	    printf("Receive Failed. Error!!!!!\n");
	    return NULL;
	}
	
	if(check_voter(client_message) == 1)
	{
		int t = 0; 
		while(client_message[t] != '/')
		{
			Name[t] = client_message[t];
			t++;
		}
		Name[t] = '\0';
		int flag = 0;
		while(flag == 0)
		{	
			memset(server_message,'\0',sizeof(server_message));
			memset(client_message,'\0',sizeof(client_message));

			poll_msg(server_message, Name);

			if (send(client_sock, server_message, strlen(server_message),0)<0)
			{
				printf("Send Failed. Error!!!!!\n");
				return NULL;
			}

		
			//Receive the message from the client
			if (recv(client_sock, client_message, sizeof(client_message),0) < 0)
			{
			    printf("Receive Failed. Error!!!!!\n");
			    return NULL;
			}

			printf("Client Message: %s\n",client_message);
			 
			//Receive User choice???????????????????
			for(int k=0; k<count; k++)
			{
				if(strcmp(canidates[k], client_message) == 0)
					flag = 1;
			}
			if(flag == 1)
			{
				save_vote(Name, client_message);
			}
			else
			{
				strcpy(server_message, "Please enter correct symbol.");
				if (send(client_sock, server_message, strlen(server_message),0)<0)
				{
					printf("Send Failed. Error!!!!!\n");
					return NULL;
				}	
			}
		}
		
		memset(server_message,'\0',sizeof(server_message));
		strcpy(server_message, "Good Bye Client!!");
		if (send(client_sock, server_message, strlen(server_message),0)<0)
		{
			printf("Send Failed. Error!!!!!\n");
			return NULL;
		}


	}
	else
	{
		memset(server_message,'\0',sizeof(server_message));
		strcpy(server_message, "You are not authorized.");
		if (send(client_sock, server_message, strlen(server_message),0)<0)
		{
			printf("Send Failed. Error!!!!!\n");
			return NULL;
		}
	}
	close(client_sock);
	clients--;
	pthread_exit(NULL);
}


int main(void)
{
	pthread_t thread[3];
	int socket_desc, client_sock, client_size; 
        struct sockaddr_in server_addr, client_addr;         //SERVER ADDR will have all the server address
        char server_message[2000], client_message[2000];     // Sending values from the server and receive from the server we need this

        //Cleaning the Buffers
        
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));     // Set all bits of the padding field//

	load_canidates();
        
        //Creating Socket
        
        socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        
        if(socket_desc < 0)
        {
                printf("Could Not Create Socket. Error!!!!!\n");
                return -1;
        }
        
        printf("Socket Created\n");
        
        //Binding IP and Port to socket
        
        server_addr.sin_family = AF_INET;               /* Address family = Internet */
        server_addr.sin_port = htons(2000);               // Set port number, using htons function to use proper byte order */
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");    /* Set IP address to localhost */
		
		
		
		// BINDING FUNCTION
        
        if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0)    // Bind the address struct to the socket.  /
	                            	//bind() passes file descriptor, the address structure,and the length of the address structure
        {
                printf("Bind Failed. Error!!!!!\n");
                return -1;
        }        
        
        printf("Bind Done\n");

	while(1)
	{
        
		//Put the socket into Listening State

		if(listen(socket_desc, 1) < 0)                               //This listen() call tells the socket to listen to the incoming connections.
		// The listen() function places all incoming connection into a "backlog queue" until accept() call accepts the connection.
		{
			printf("Listening Failed. Error!!!!!\n");
			return -1;
		}

		printf("Listening for Incoming Connections.....\n");

		//Accept the incoming Connections

		client_size = sizeof(client_addr);
	
	
	
		client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);          // heree particular client k liye new socket create kr rhaa ha

		if (client_sock < 0)
		{
			printf("Accept Failed. Error!!!!!!\n");
			return -1;
		}

		printf("Client Connected with IP: %s and Port No: %i\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
			       //inet_ntoa() function converts the Internet host address in, given in network byte order, to a string in IPv4 dotted-decimal notation


		if(clients < 3)
		{

			int ret = pthread_create(&thread[clients], NULL, client_req, client_sock);
			if (ret!=0)
			{
				printf("Error In Creating Thread with ID:%d\n",clients);
			}
			else
			{
				printf("Thread is created for this client\n");
				clients++;
			}

		}
		else
		{
			memset(client_message,'\0',sizeof(client_message));
			if (recv(client_sock, client_message, sizeof(client_message),0) < 0)
			{
			    printf("Receive Failed. Error!!!!!\n");
			    return NULL;
			}
			printf("%s\n", client_message);
			
			strcpy(server_message, "Server is full.");
			if (send(client_sock, server_message, strlen(server_message),0)<0)
			{
				    printf("Send Failed. Error!!!!!\n");
				    return -1;
			}
		}
		    
		memset(server_message,'\0',sizeof(server_message));
		memset(client_message,'\0',sizeof(client_message));
        }
              
        //Closing the Socket
        
        close(client_sock);
        close(socket_desc);
        return 0;       
}