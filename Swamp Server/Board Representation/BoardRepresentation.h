#pragma once
#include "bitboards.h"
#include "Common.h"
#include "FEN_parser.h"
#include "gameHistory.h"
#include "../evaluation/evaluation.h"



Pieces _chessBoard[64];
ZorbistKeys _globalZorbistHashing={};
GameState _globalGameState;

void printPieceBitBoard(__uint64_t PIECE_BITBOARD , char CHAR)
{
    for(int i=63 ;i>=0; i--)
    {
        if(PIECE_BITBOARD & (1ULL << i)){
            printf("%c ",CHAR );
        }else{
            printf(". ");
        }
        if(i%8==0){
            printf("\n");
        }
    }
}


void _initializer(){
    generateZorbistNumbers();
    _globalGameState=initiaizeNewGame();
    _globalGameState._evalValue=evaluateThisPosition(_globalGameState);
    pieceInitializer();
    piecePuter(_chessBoard, _globalGameState._blackPawns, BLACK_PAWN);
    piecePuter(_chessBoard, _globalGameState._blackRooks, BLACK_ROOK);
    piecePuter(_chessBoard, _globalGameState._blackBishops, BLACK_BISHOP);
    piecePuter(_chessBoard, _globalGameState._blackKnights, BLACK_KNIGHT);
    piecePuter(_chessBoard, _globalGameState._blackQueens, BLACK_QUEEN);
    piecePuter(_chessBoard, _globalGameState._blackKing, BLACK_KING);
    piecePuter(_chessBoard, _globalGameState._whitePawns, WHITE_PAWN);
    piecePuter(_chessBoard, _globalGameState._whiteRooks, WHITE_ROOK);
    piecePuter(_chessBoard, _globalGameState._whiteBishops, WHITE_BISHOP);
    piecePuter(_chessBoard , _globalGameState._whiteKnights, WHITE_KNIGHT);
    piecePuter(_chessBoard, _globalGameState._whiteQueens, WHITE_QUEEN);
    piecePuter(_chessBoard, _globalGameState._whiteKing, WHITE_KING);
    piecePuter(_chessBoard , ~(_globalGameState._occupancy) , ES);
}


void initializerFromFen(char* FEN_STRING){

    _globalGameState=initializeNewGameFromString(FEN_STRING);
    _globalGameState._evalValue=evaluateThisPosition(_globalGameState);
    pieceInitializer();

    piecePuter(_chessBoard, _globalGameState._blackPawns, BLACK_PAWN);
    piecePuter(_chessBoard, _globalGameState._blackRooks, BLACK_ROOK);
    piecePuter(_chessBoard, _globalGameState._blackBishops, BLACK_BISHOP);
    piecePuter(_chessBoard, _globalGameState._blackKnights, BLACK_KNIGHT);
    piecePuter(_chessBoard, _globalGameState._blackQueens, BLACK_QUEEN);
    piecePuter(_chessBoard, _globalGameState._blackKing, BLACK_KING);
    piecePuter(_chessBoard, _globalGameState._whitePawns, WHITE_PAWN);
    piecePuter(_chessBoard, _globalGameState._whiteRooks, WHITE_ROOK);
    piecePuter(_chessBoard, _globalGameState._whiteBishops, WHITE_BISHOP);
    piecePuter(_chessBoard , _globalGameState._whiteKnights, WHITE_KNIGHT);
    piecePuter(_chessBoard, _globalGameState._whiteQueens, WHITE_QUEEN);
    piecePuter(_chessBoard, _globalGameState._whiteKing, WHITE_KING);
    piecePuter(_chessBoard , ~_globalGameState._occupancy , ES);
}



void printBoard()
{
    for (int i = 64; i > 0; i--)
    {
            switch(_chessBoard[i-1]){
                case BLACK_ROOK:
                    printf("%c ",'r');
                    break;
                case BLACK_BISHOP:
                    printf("%c ",'b');
                    break;
                case BLACK_KING:
                    printf("%c ",'k');
                    break;
                case BLACK_KNIGHT:
                    printf("%c ",'n');
                    break;
                case BLACK_PAWN:
                    printf("%c ",'p');
                    break;
                case BLACK_QUEEN:
                    printf("%c ",'q');
                    break;
                case WHITE_ROOK:
                    printf("%c ",'R');
                    break;
                case WHITE_BISHOP:
                    printf("%c ",'B');
                    break;
                case WHITE_KING:
                    printf("%c ",'K');
                    break;
                case WHITE_KNIGHT:
                    printf("%c ",'N');
                    break;
                case WHITE_QUEEN:
                    printf("%c ",'Q');
                    break;
                case WHITE_PAWN:
                    printf("%c ",'P');
                    break;
                case ES:
                    printf("%c ",'.');
                    break;
                    break;
                default:
                    break;
            }
        
            if(i%8==1 )
            {
                printf("\n");
            }
        
    }
}



void printGameStruct()
{
    printBoard();
    printf("Castling left --> %d\n", _globalGameState._castlingAvailable);
    printf("Half Moves --> %d\n", _globalGameState._numberHalfMoves);
    printf("Full Moves --> %d\n", _globalGameState._numberMoves);
    printf("Piece To Move -> %c\n", _globalGameState._pieceToMove);
    printf("Enpassant FIle -> %c\n", _globalGameState._enpassantFile);
    printf("Zorbist Hash -> %ld\n" , _globalGameState._zobristHash);
    printf("Eval Value -> %lf\n" , _globalGameState._evalValue);
    printf("Black Occupancy -> %ld\n", _globalGameState._blackOccupancy);
    printf("White Occupancy -> %ld\n", _globalGameState._whiteOccupancy);

}