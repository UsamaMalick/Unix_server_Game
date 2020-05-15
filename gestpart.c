
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
int **ship_coordinates;




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



void SharedMemory_Data(){

	strcpy(shmp->buf,"===============\n|  ");
	strcat(shmp->buf,"Score Board: \n");
	for(int i = 0; i < num_of_users; i++) {
		strcat(shmp->buf,"|  ");
		

		char* str1;
		asprintf (&str1, "%s", name_of_users[i]);
		strcat(shmp->buf,str1);
		free(str1);

		//strcat(shmp->buf , "\t");
		strcat(shmp->buf,": ");

		

		char* str;
		asprintf (&str, "%i", user_scores[i]);
		strcat(shmp->buf,str);
		free(str);
		strcat(shmp->buf , "\t");
		strcat(shmp->buf,"\n");
	}
	strcat(shmp->buf,"===============\n");
	
	
	printf("%s" , shmp->buf);

}


void saveDataToSharedMemory(){
	

	   bufptr = shmp->buf;
	   spaceavailable = BUF_SIZE;
	   
	   shmp->cnt = fill_buffer(bufptr, spaceavailable);
	   shmp->complete = 0;
	   spaceavailable = BUF_SIZE;
	  
	   shmp->complete = 1;

	   
}

int isGameOver(){
// return 0 means "game over" | return 1 means "game not over" 
	for(int i=0;i<5;i++){
		for(int j=0;j<5;j++){
			if(board_array[i][j]!=0){
				return 1;
			}
		}
	}
	return 0;
}




void admin_setup(){
    
    for(int i=0;i<5;i++){
	for(int j=0;j<5;j++){
		board_array[i][j]=0;
	}
    }

    int pow;
    int flag = 0;
    while(flag == 0)
    {
        printf("Kindly enter number of ships(In range 1-10):");
        scanf("%d", &num_of_ships);

        if(! (num_of_ships >= 1 && num_of_ships <= 10) ){
            printf("Wrong number of ships entered.\n");
        }
        else{
            flag = 1;

		//row, col, position, pow
		ship_coordinates = (int*) calloc(num_of_ships, sizeof(int));
		for ( int i = 0; i < num_of_ships; i++ )
		{
		    ship_coordinates[i] = (int*) calloc(4, sizeof(int)); // 100 represents the size
		}
        }
    }

	//2 means Ship exist, 1 means ship hit, 0 means water 
	char position;
	for(int i=0; i<num_of_ships; i++){
        flag = 0;
        while(flag == 0)
        {
			printf("Enter ship position(V for vertical/H for horizontal):");
			scanf(" %c", &position);

			if(position == 'V'|| position == 'H')
			{
				printf("\n\nEnter ship %d start coordinates(In range 1-5).\n", i+1);

		        printf("Row:");
		        scanf("%d", &row);

		        printf("Column:");
		        scanf("%d", &col);

			int flag1 = 1; //For checking already placed ship
		        if( (row >= 1 && row <= 5) && (col >= 1 && col <= 5) ){
		            printf("Power (In range 1-5) : ");
		            scanf("%d", &pow);

		            if(pow >= 1 && pow <= 5){

						int pos;
						if(position == 'H' && pow+col-1<=5)
						{
							flag1 = 0;
							pos = 1; //1 means ship is veritcal
						}
						else if(position == 'V' && pow+row-1<=5)
						{
							flag1 = 0; 
							pos = 0; //0 means ship is horizontal
						}
					
						if(flag1 == 0)
						{
							ship_coordinates[i][0] = row;
							ship_coordinates[i][1] = col;
							ship_coordinates[i][2] = pos;
							ship_coordinates[i][3] = pow;
							int flag2 = 1; 
							for(int k=0; k<pow; k++)
							{
								if(position == 'V' && board_array[row-1+k][col-1] == 0){
									flag2 = 0;
								}
								else if(position == 'H' && board_array[row-1][col-1+k] == 0){
									flag2 = 0;
								}
								else{
									flag2 = 1;
									break;
								}
							}
							
							if(flag2 == 0)
							{
								for(int k=0; k<pow; k++)
								{
									if(position == 'V' && board_array[row-1+k][col-1] == 0){
									    flag = 1;
									    board_array[row-1+k][col-1] = 2;

									}
									else if(position == 'H' && board_array[row-1][col-1+k] == 0)
									{
									    flag = 1;
									    board_array[row-1][col-1+k] = 2;
									}
								}
								printBoard();
							}
							else
							{
								printf("There is already a ship.\n");
							}
			
						}
						else{
							printf("The ship is out of board.\n");
						}

		            }
		            else{
		                printf("Power Should be in range of 1-5.\n");
		            }   
		        }
		        else{
		            printf("Wrong coordinates entered. Kindly enter values in 1-5 range.\n");
		        }

			}

            
        }
        
	}

    flag = 0;
    while(flag == 0)
    {
        printf("Kindly enter number of users (Minimum 2 users) : ");
	    scanf("%d", &total_allowed_users);

        if(! (total_allowed_users >= 2)){
            printf("Wrong number of user entered.\n");
        }
        else{
            flag = 1;
        }
    }
   printf("Your Game board is Ready.\n");
   printBoard();
}

    
void SavingUser(char * username) {

    for (int i=0; i<strlen(username); i++) {
    	name_of_users[num_of_users][i] = username[i];   
    }

}


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


int fill_buffer(char * bufptr, int size) {
   
   int filled_count;

   memset(bufptr , '\0', 0*sizeof(char)); 
   bufptr[size-1] = '\0';
   
   filled_count = strlen(bufptr);
   return filled_count;
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

	///////////////////////////
	if (shmdt(shmp) == -1) {
		      perror("shmdt");
		      return 1;
		   }

	   if (shmctl(shmid, IPC_RMID, 0) == -1) {
	      perror("shmctl");
	      return 1;
	   }
}

void initialize_Variables(){

	num_of_users = 0;

	name_of_users = (char*) calloc(total_allowed_users, sizeof(char));
	
	
	
	for ( int i = 0; i < total_allowed_users; i++ )
	{
	    name_of_users[i] = (char*) calloc(100, sizeof(char)); // 100 represents the size
	}
	////////////////////////////
	user_scores = (int*) malloc(total_allowed_users* sizeof(int));
	
	
	
	cient_sock_arr = (int*) malloc(total_allowed_users * sizeof(int));
	 
	// if memory cannot be allocated
	if(user_scores == NULL)                     
	{
		printf("Error! memory not allocated.");
		exit(0);
	}
	
	for ( int i = 0; i < total_allowed_users; i++ )
	{
	    user_scores[i] = 0; // 100 represents the size
	}
	
	////////////////////
	
	shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0644|IPC_CREAT);
	
   	if (shmid == -1) {
   	
      		perror("Shared memory");
      		return 1;
   	}
   
   	// Attach to the segment to get a pointer to it.
   	shmp = shmat(shmid, NULL, 0);
   	if (shmp == (void *) -1) {
      		perror("Shared memory attach");
      		return 1;
   	}
	/////////////////////
	
	
}

bool allow(char * username){
    
    char allowUser;
    
    printf("\nDo you want to allow user to enter in the game room. \n");
    printf("User name is : ");
    printf("%s\n" , username);
    printf("Please Select an option [y/n] : ");
    
    scanf(" %c" , &allowUser);
    
    
    //printf("%c" , allowUser);
    
    if (allowUser == 'y') {
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
      
      
void coodinates(char buf[], int *row, int *col)
{
	*row = (int)buf[0] - 48;
	*col = (int)buf[1] - 48;
}


char* Checkwin()
{
	int i, max = user_scores[0], count = 0 , index = 0;
	

	for(i=1; i<total_allowed_users; i++)
	{
		
		if(max < user_scores[i])
		{
			index = i;
			max = user_scores[i];
			count = 0;
		}
		else if(max == user_scores[i])
		{
			count++;
		}
	}
	
	char temp[100];
	temp[0] = '\0';
	char* name = temp;

	if(count == 0)
	{
		strcpy(name, name_of_users[index]);
		strcat(name, " Wins !!");
	}
	else
	{
		strcpy(name, "Tie !!!");
	}
	return name;
}

