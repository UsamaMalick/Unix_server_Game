

#include "protoclient.h"


int shmid;
struct shmseg *shmp;




void clear_screen(){
    system("clear");
}



void initialize_variables(){

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

}

void freeResources(){

		if (shmp->cnt == -1) {
		 perror("read");
		 return 1;
	      }
   // printf("Reading Process: Reading Done, Detaching Shared Memory\n");
	   if (shmdt(shmp) == -1) {
	      perror("shmdt");
	      return 1;
		}
}

char* client()
{
    static char arr[3];
    int row, col;
    int flag = 0;
    while(flag == 0)
    {
        printf("Enter coordinates(In range 1-5) to Attack.\n");

        printf("Row:");
        scanf("%d", &row);

        printf("Column:");
        scanf("%d", &col);

        if( (row >= 1 && row <= 5) && (col >= 1 && col <= 5) )
        {
            arr[0] = row + 48; 
            arr[1] = col + 48;
            arr[2] = '\0';
            flag = 1;
        }
        else 
        {
            printf("The coordinates are out of range.\n");
        }
    }
    //printf("%s", arr);
    return arr;
}


void retrieveDataFromSharedMemory(){
	
	      printf("segment contains : \n\"%s\"\n", shmp->buf);
}
