
#include <stdio.h> 
#include <string.h> 
# include <stdlib.h>
# include <stdbool.h>
#include <ctype.h>
#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr


int main(void)
{
        int socket_desc, client_sock, client_size; 
        struct sockaddr_in server_addr, client_addr;         
	//SERVER ADDR will have all the server address
        char server_message[2000], client_message[2000];                 
	// Sending values from the server and receive 
	//from the server we need this
        //Cleaning the Buffers
        
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
	server_addr, 	sizeof(server_addr))<0)    
	// Bind the address struct to the socket.  /
	//bind() passes file descriptor, the address structure,
	//and the length of the address structure
        {
                printf("Bind Failed. Error!!!!!\n");
                return -1;
        }        
        
        printf("Bind Done\n");
        
        //Put the socket into Listening State

	 
        if(listen(socket_desc, 1) < 0)                              
	 // This listen() call tells the socket to listen 
	 // to the incoming connections.
	 // The listen() function places all incoming 
	 // connection into a "backlog queue" until accept()
	 // call accepts the connection.
         {
                printf("Listening Failed. Error!!!!!\n");
                return -1;
         }
        
        
        //Accept the incoming Connections
        
        client_size = sizeof(client_addr);

	while(1){	
	
	    printf("Listening for Incoming Connections.....\n");
		
	    client_sock = accept(socket_desc, 
	    (struct sockaddr*)&client_addr, &client_size);
		
		if (client_sock < 0)
		{
		        printf("Accept Failed. Error!!!!!!\n");
		        return -1;
		}
		
		printf("Client Connected with IP: %s and Port No: %i\n", 					inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
	       //inet_ntoa() function converts the Internet  			   
		//host address in, given in network byte order, 
		//to a string in IPv4 dotted-decimal notation

	    if (recv(client_sock, client_message, sizeof(client_message),0) < 0)
		{
		        printf("Receive Failed. Error!!!!!\n");
		        return -1;
		}
		
		printf("Client Message: %s\n",client_message);
		char number_array [100] = {};
		
		int size = sizeof(client_message);
		int k = 0;
		for (int i=0; i<size; i++)
		{	
			
			//printf("%s" , client_message[i]);
			if(client_message[i] >= '0' 
			&& client_message[i] <= '9')
			{
				number_array[k] = client_message[i]- 48;
				k++;
			}
		}
	}

}
        
