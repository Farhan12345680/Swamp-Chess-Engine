#include "./Board Representation/BoardRepresentation.h"
#include "./Move Generation/MoveGeneration.h"
#include "./Move Generation/PieceMovement.h"
#include <string.h>
#include <stdio.h>




// int main(){
        //initializer();
//     char * _str =(char*)malloc(128);

//     while(true)
//     {
//         fgets( _str,128,stdin);
//         _str[strcspn(_str , "\r\n" )]='\0';
//         if (strcmp(_str, "uci") == 0)
//         {
//             printf("Swamp Chess Enigne 0.0.1\n");
//             printf("Made By Farhan Adib @Farhan12345680\n");
//             printf("END\n");

//             fflush(stdout);        
//         }
//         else if(strcmp(_str, "2") == 0){
//                 initializer();
//                 printf("END\n");

//                 fflush(stdout);    

//         }
//         else if(strcmp(_str, "3") == 0){
//             char* _str1 = (char*)malloc(MAX_UCI_QUERY_LENGTH);
//             fgets(_str1 ,128,stdin );

//             _str1[strcspn(_str1, "\r\n")]='\0';

//             if(fenStringValidator(_str1)){
//                 initializerFromFen(_str1);
//                 printf("initialized from FEN\n");

//             }else
//             {
//                 initializer();
//                 printf("initialized\n");

//             }
//             printf("END\n");
//             fflush(stdout); 

//             free(_str1);
//         }
//         else if(strcmp(_str, "4") == 0)
//         {   
//             printf("printing Board\n");
//             printBoard();
//             printf("END printing Board\n");

//             printf("END\n");
//             fflush(stdout);
//         }
//         else if(strcmp(_str , "12")==0)
//         {
//             char* _str1 = (char*)malloc(MAX_UCI_QUERY_LENGTH );
//             fgets(_str1 ,128,stdin );

//             _str1[strcspn(_str1, "\r\n")]='\0';

//             if(strcmp(_str1 ,"BLACK PAWN")==0)
//             {
//                 printPieceBitBoard(_globalGameState._blackPawns , 'p');
//             }else if(strcmp(_str1 ,"BLACK KING")==0){
//                 printPieceBitBoard(_globalGameState._blackKing , 'k');

//             }else if(strcmp(_str1 ,"BLACK QUEEN")==0){
//                 printPieceBitBoard(_globalGameState._blackQueens, 'q');

//             }else if(strcmp(_str1 ,"BLACK BISHOP")==0){
//                 printPieceBitBoard(_globalGameState._blackBishops, 'b');

//             }else if(strcmp(_str1 ,"BLACK ROOK")==0){
//                 printPieceBitBoard(_globalGameState._blackRooks, 'r');
//             }else if(strcmp(_str1 ,"WHITE PAWN")==0){
//                 printPieceBitBoard(_globalGameState._whitePawns, 'P');
//             }else if(strcmp(_str1 ,"WHITE KING")==0){
//                 printPieceBitBoard(_globalGameState._whiteKing, 'K');
//             }else if(strcmp(_str1 ,"WHITE QUEEN")==0){
//                 printPieceBitBoard(_globalGameState._whiteQueens, 'Q');
//             }else if(strcmp(_str1 ,"WHITE BISHOP")==0){
//                 printPieceBitBoard(_globalGameState._whiteBishops, 'B');
//             }else if(strcmp(_str1 ,"WHITE ROOK")==0){
//                 printPieceBitBoard(_globalGameState._whitePawns, 'R');

//             }

//             printf("END\n");
//             fflush(stdout); 

//             free(_str1);
//         }
//     }

//     free(_str);


//     return 0;
// }



int main(){
    _initializer();

    for(int i=1;i<=8; i++){
        printf("perft %d is %ld\n",i,perft(&_globalGameState , i));

    }
}