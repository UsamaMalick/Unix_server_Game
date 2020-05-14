

#include "protoclient.h"

int shmid;
struct shmseg *shmp;


int main(void)
{
        initialize_variables();
        int socket_desc;
        struct sockaddr_in server_addr;
        char server_message[2000], client_message[2000];
        char username[100], password[100];
        
        //Cleaning the Buffers
        
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));
        
        clear_screen();
        
        //Creating Socket
        
        socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        
        if(socket_desc < 0)
        {
                printf("Could Not Create Socket. Error!!!!!\n");
                return -1;
        }
        
        printf("Socket Created\n");
        
        //Specifying the IP and Port of the server to connect
        
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(2001);
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        
        //Now connecting to the server accept() using connect() from client side
        
        if(connect(socket_desc, (struct sockaddr*) & 
	server_addr, sizeof(server_addr)) < 0)
        {
                printf("Connection Failed. Error!!!!!");
                return -1;
        }
        
        printf("Connected\n");
        
        //Get Input from the User
        printf("Give User Name : \n");
        gets(username);
        printf("Give Password  : \n");
        gets(password);
        
        int l = 0;
        for(int i=0; i<strlen(username); i++){
        	client_message[l++] = username[i];
        
        }

        client_message[l++] = '@';
        for(int i = 0; i<strlen(password); i++) {
        	client_message[l++] = password[i];
        }
        client_message[l++] = '\n';
        //gets(client_message);   
         
        //readFile(client_message);
        
        
        //strcpy (client_message , readFile());                               
        //Send the message to Server
        
        if(send(socket_desc, client_message, strlen(client_message),0) < 0)
        {
                printf("Send Failed. Error!!!!\n");
                return -1;
        }
 
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));   
	//Closing the Socket
	
	
	
        if(recv(socket_desc, server_message, sizeof(server_message),0) < 0)
        {
                printf("Receive Failed. Error!!!!!\n");
                return -1;
        }
        
        printf("\n%s\n",server_message);
        
        
        clear_screen();
        
        bool enter_room = false;
        
        if(server_message[0] == '1'){       
        	enter_room = true;
        	printf("Welcome to the GameZone\n");
        }
        else printf("You are not allowed to enter the GameZone.\n");
        
        
        
        
        while(enter_room == true) {
        
        	printf("Into the Game!.\n");
        	
   
        	memset(client_message,'\0',sizeof(client_message));         	
         	memset(server_message,'\0',sizeof(server_message)); 
         	
         	
         	if(recv(socket_desc, server_message, sizeof(server_message),0) < 0)
	{
	 	printf("Receive Failed. Error!!!!!\n");
	  	return -1;
	}
	
	
	printf("\n%s\n",server_message);
	
	
	if (strcmp(server_message ,  "Get Ready for you Turn. The game has started!") != 0){
        		retrieveDataFromSharedMemory(); 		
        	
        	}
        	
        	memset(server_message,'\0',sizeof(server_message)); 
 
	if(recv(socket_desc, server_message, sizeof(server_message),0) < 0)
	{
	 	printf("Receive Failed. Error!!!!!\n");
	  	return -1;
	}

	printf("Server Message: \n%s\n",server_message);

        	
        	if (strcmp(server_message , "Take Your Turn!") != 0){
        		retrieveDataFromSharedMemory();
        		enter_room = false;
        		break;    		
        		
        	}
        	retrieveDataFromSharedMemory();
        	
  
        	
        	
        	printf("Sending to Server .\n");
        	strcpy(client_message, client());
        	
        	if(send(socket_desc, client_message, strlen(client_message),0) < 0)
	{
		printf("Send Failed. Error!!!!\n");
		return -1;
	}  
		        
        }
        
        
      
        printf("Have a Good Day.Bye!  -_-\n");
  
        freeResources();        
        return 0;
}

