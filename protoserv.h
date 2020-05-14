

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


#define BUF_SIZE 1024
#define SHM_KEY 0x1234



struct shmseg {
   int cnt;
   int complete;
   char buf[BUF_SIZE];
};


extern int shmid, numtimes;
extern struct shmseg *shmp;
extern char *bufptr;
extern int spaceavailable;
extern int total_allowed_users;
extern int num_of_users;
extern char **name_of_users;
extern int *user_scores;
extern int board_array[5][5];
extern int num_of_ships , row, col;
extern int *cient_sock_arr;


void printScoreBoard();

void SharedMemory_Data();

void saveDataToSharedMemory();

int isGameOver();

void admin_setup();

void SavingUser(char * username) ;

void clear_screen();

void updateScore(int user_index,int score);

int attack(int row, int column,int user_index);

int fill_buffer(char * bufptr, int size);

void printBoard();

void freeResources();

void initialize_Variables();

bool allow(char * username);

bool isUser(char * UserIncoming);

void coodinates(char buf[], int *row, int *col);

char* Checkwin();





