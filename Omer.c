#include <stdio.h>

int board_array[5][5];
int num_of_ships = 0, total_allowed_users = 0, row, col;

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

        if(! (num_of_ships >= 1 || num_of_ships <= 10) ){
            printf("Wrong number of ships entered.\n");
        }
        else{
            flag = 1;
        }
    }

	for(int i=0; i<num_of_ships; i++){
        flag = 0;
        while(flag == 0)
        {
            printf("Enter ship %d coordinates(In range 1-5).\n", i+1);

            printf("Row:");
            scanf("%d", &row);

            printf("Column:");
            scanf("%d", &col);

            if( (row >= 1 || row <= 5) && (col >= 1 || col <= 5) ){
                printf("Power:");
                scanf("%d", &pow);

                if(pow >= 1 || pow <= 5){
                    if(board_array[row-1][col-1] == 0){
                        flag = 1;
                        board_array[row-1][col-1] = pow;
                        printBoard();
                    }
                    else{
                        printf("There is already a ship.");
                    }
                }
                else{
                    printf("Power Should be in range of 1-5.");
                }   
            }
            else{
                printf("Wrong coordinates entered. Kindly enter values in 1-5 range.\n");
            }
        }
        
	}

    flag = 0;
    while(flag == 0)
    {
        printf("Kindly enter number of users(In range 1-3):");
	    scanf("%d", &total_allowed_users);

        if(! (total_allowed_users >= 1 || total_allowed_users <= 3)){
            printf("Wrong number of user entered.\n");
        }
        else{
            flag = 1;
        }
    }
	
	printBoard();
}


int main() {
   // printf() displays the string inside quotation
   printf("Hello, World!");
   return 0;
}