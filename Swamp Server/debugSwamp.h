#pragma once
#include <string.h>
#include "Swamp.h"
#include <stdio.h>

void makeMove(char *arr)
{
    int src  = ((arr[0]+0 - 'a' ) + (arr[1]+0 - '1')*8);
    int dest = ((arr[2] - 'a') + (arr[3]+0 - '1')*8 );

    uint8_t srcColor, srcPiece;
    uint8_t destColor = NO_PIECE;
    uint8_t destPiece = NO_PIECE;

    switch (_chessBoard[src])
    {
        case WHITE_PAWN:   srcColor = WHITE_OCCUPANCY; srcPiece = WHITE_PAWN_OCCUPANCY; break;
        case WHITE_KNIGHT: srcColor = WHITE_OCCUPANCY; srcPiece = WHITE_KNIGHT_OCCUPANCY; break;
        case WHITE_BISHOP: srcColor = WHITE_OCCUPANCY; srcPiece = WHITE_BISHOP_OCCUPANCY; break;
        case WHITE_ROOK:   srcColor = WHITE_OCCUPANCY; srcPiece = WHITE_ROOK_OCCUPANCY; break;
        case WHITE_QUEEN:  srcColor = WHITE_OCCUPANCY; srcPiece = WHITE_QUEEN_OCCUPANCY; break;
        case WHITE_KING:   srcColor = WHITE_OCCUPANCY; srcPiece = WHITE_KING_OCCUPANCY; break;

        case BLACK_PAWN:   srcColor = BLACK_OCCUPANCY; srcPiece = BLACK_PAWN_OCCUPANCY; break;
        case BLACK_KNIGHT: srcColor = BLACK_OCCUPANCY; srcPiece = BLACK_KNIGHT_OCCUPANCY; break;
        case BLACK_BISHOP: srcColor = BLACK_OCCUPANCY; srcPiece = BLACK_BISHOP_OCCUPANCY; break;
        case BLACK_ROOK:   srcColor = BLACK_OCCUPANCY; srcPiece = BLACK_ROOK_OCCUPANCY; break;
        case BLACK_QUEEN:  srcColor = BLACK_OCCUPANCY; srcPiece = BLACK_QUEEN_OCCUPANCY; break;
        case BLACK_KING:   srcColor = BLACK_OCCUPANCY; srcPiece = BLACK_KING_OCCUPANCY; break;

        default:
            printf("No piece on source square.\n");
            return;
    }

    switch (_chessBoard[dest])
    {
        case ES:
            break;

        case WHITE_PAWN:   destColor = WHITE_OCCUPANCY; destPiece = WHITE_PAWN_OCCUPANCY; break;
        case WHITE_KNIGHT: destColor = WHITE_OCCUPANCY; destPiece = WHITE_KNIGHT_OCCUPANCY; break;
        case WHITE_BISHOP: destColor = WHITE_OCCUPANCY; destPiece = WHITE_BISHOP_OCCUPANCY; break;
        case WHITE_ROOK:   destColor = WHITE_OCCUPANCY; destPiece = WHITE_ROOK_OCCUPANCY; break;
        case WHITE_QUEEN:  destColor = WHITE_OCCUPANCY; destPiece = WHITE_QUEEN_OCCUPANCY; break;
        case WHITE_KING:   destColor = WHITE_OCCUPANCY; destPiece = WHITE_KING_OCCUPANCY; break;

        case BLACK_PAWN:   destColor = BLACK_OCCUPANCY; destPiece = BLACK_PAWN_OCCUPANCY; break;
        case BLACK_KNIGHT: destColor = BLACK_OCCUPANCY; destPiece = BLACK_KNIGHT_OCCUPANCY; break;
        case BLACK_BISHOP: destColor = BLACK_OCCUPANCY; destPiece = BLACK_BISHOP_OCCUPANCY; break;
        case BLACK_ROOK:   destColor = BLACK_OCCUPANCY; destPiece = BLACK_ROOK_OCCUPANCY; break;
        case BLACK_QUEEN:  destColor = BLACK_OCCUPANCY; destPiece = BLACK_QUEEN_OCCUPANCY; break;
        case BLACK_KING:   destColor = BLACK_OCCUPANCY; destPiece = BLACK_KING_OCCUPANCY; break;
        default:
            break;
    }
    
    doMove(src,
           dest,
           0,              
           srcColor,
           srcPiece,
           destColor,
           destPiece , ES);
}


void swampDebugServer()
{
    char array[128];

    while (1)
    {
        fgets(array, sizeof(array), stdin);
        array[strcspn(array, "\n")] = '\0';

        if (!strcmp(array, "exit"))
            break;
        if(!strcmp(array ,"0")){
            initializer();
        }
        if (!strcmp(array, "1"))
        {
            initializer();

            fgets(array, sizeof(array), stdin);
            array[strcspn(array, "\n")] = '\0';

            makeMove(array);
            printf("%s\n", array);
        }
        else if (!strcmp(array, "2"))
        {
            fgets(array, sizeof(array), stdin);
            array[strcspn(array, "\n")] = '\0';

            makeMove(array);
        }
        else if (!strcmp(array, "3"))
        {
            fgets(array, sizeof(array), stdin);
            array[strcspn(array, "\n")] = '\0';

            divide(atoi(array));
        }else if(!strcmp(array , "4")){
            printBoard();
        }
        else if (!strcmp(array, "5"))
        {
           printf( "--> %ld \n",perft(6));;
        }
        else if(!(strcmp(array , "6"))){
            printPieceBitBoard(GAME_STATE[WHITE_OCCUPANCY] ,'w');
        }
                else if(!(strcmp(array , "7"))){
                GAME_STATE[SIDE]=0;
            printPieceBitBoard(getAttackBitBoard(0 , 7) ,'w');
        }
        else if(!(strcmp(array , "8"))){
            GAME_STATE[SIDE]=6;
            printPieceBitBoard(getAttackBitBoard(6 , 0) ,'b');
        }
        else if(!(strcmp(array , "bk"))){

            printPieceBitBoard(GAME_STATE[BLACK_KING_OCCUPANCY], 'b');
        }
        else if(!(strcmp(array , "WK"))){
            printPieceBitBoard(GAME_STATE[WHITE_KING_OCCUPANCY] ,'w');
        }
        else if(!(strcmp(array , "WK"))){
            printPieceBitBoard(GAME_STATE[WHITE_KING_OCCUPANCY] ,'w');
        }
    }
}