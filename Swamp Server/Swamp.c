#include "./Board Representation/BoardRepresentation.h"
#include "./Move Generation/MoveGeneration.h"
#include "./Move Generation/PieceMovement.h"
#include <string.h>
#include <stdio.h>




int main(){
    initializer();
    char * _str =(char*)malloc(128);

    while(true)
    {
        fgets( _str,128,stdin);
        _str[strcspn(_str , "\r\n" )]='\0';
        if (strcmp(_str, "uci") == 0)
        {
            printf("Swamp Chess Enigne 0.0.1\n");
            printf("Made By Farhan Adib @Farhan12345680\n");
            printf("END\n");

            fflush(stdout);        
        }
        else if(strcmp(_str, "2") == 0){
                initializer();
                printf("END\n");

                fflush(stdout);    

        }
        else if(strcmp(_str, "3") == 0){
            char* _str1 = (char*)malloc(MAX_UCI_QUERY_LENGTH);
            fgets(_str1 ,128,stdin );

            _str1[strcspn(_str1, "\r\n")]='\0';

            if(fenStringValidator(_str1)){
                initializerFromFen(_str1);
                printf("initialized from FEN\n");

            }else
            {
                initializer();
                printf("initialized\n");

            }
            printf("END\n");
            fflush(stdout); 

            free(_str1);
        }
        else if(strcmp(_str, "4") == 0)
        {   
            printf("printing Board\n");
            printBoard();
            printf("END printing Board\n");

            printf("END\n");
            fflush(stdout);
        }

    }

    free(_str);


    return 0;
}
