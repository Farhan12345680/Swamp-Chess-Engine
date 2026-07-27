#pragma once
#include "Common.h"

typedef struct
{
    __uint64_t _blackPawns;
    __uint64_t _blackRooks;
    __uint64_t _blackBishops;
    __uint64_t _blackKnights;
    __uint64_t _blackQueens;
    __uint64_t _blackKing;

    __uint64_t _whitePawns;
    __uint64_t _whiteRooks;
    __uint64_t _whiteKnights;
    __uint64_t _whiteBishops;
    __uint64_t _whiteQueens;
    __uint64_t _whiteKing;

    __uint64_t _blackOccupancy;
    __uint64_t _whiteOccupancy;
    __uint64_t _occupancy;

    __uint64_t _zobristHash;
    double _evalValue;


    int  _numberHalfMoves;
    int _numberMoves;

    uint8_t _castlingAvailable;
    char _pieceToMove;

    char _enpassantFile ;

} GameState;


typedef struct
{
    __uint64_t _zorbistPieces[12][64];
    __uint64_t _zorbistSideToMove;
    __uint64_t _zorbistCastlingNums[16];
    __uint64_t _zobistFileNums[8];
    
} ZorbistKeys ;

typedef enum
{
    WHITE_KING,
    WHITE_QUEEN,
    WHITE_BISHOP,
    WHITE_KNIGHT,
    WHITE_ROOK,
    WHITE_PAWN,
    BLACK_KING,
    BLACK_QUEEN,
    BLACK_BISHOP,
    BLACK_KNIGHT,
    BLACK_ROOK,
    BLACK_PAWN,
    ES
} Pieces;

extern Pieces _chessBoard[64];
extern ZorbistKeys _globalZorbistHashing;


//function
ZorbistKeys generateZorbistNumbers();
__uint64_t generateZorbistHashFromAGameState(GameState GAME_STATE);



typedef enum
{
    A1 , B1 , C1 , D1 , E1 , F1 , G1 , H1,
    A2 , B2 , C2 , D2 , E2 , F2 , G2 , H2,
    A3 , B3 , C3 , D3 , E3 , F3 , G3 , H3,
    A4 , B4 , C4 , D4 , E4 , F4 , G4 , H4,
    A5 , B5 , C5 , D5 , E5 , F5 , G5 , H5,
    A6 , B6 , C6 , D6 , E6 , F6 , G6 , H6,
    A7 , B7 , C7 , D7 , E7 , F7 , G7 , H7,
    A8 , B8 , C8 , D8 , E8 , F8 , G8 , H8,

} Square;

;



// PRNG (Pseudo Random Number Generator)
__uint64_t pseudoRandomNumberGenerator(__uint64_t* seed){

    __uint64_t _z = (*seed += 0x9E3779B97F4A7C15ULL);

    _z = (_z ^ (_z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    _z = (_z ^ (_z >> 27)) * 0x94D049BB133111EBULL;

    return _z ^ (_z >> 31);
}


GameState initiaizeNewGame()
{

    _globalZorbistHashing = generateZorbistNumbers(); 
    GameState _newState;
    _newState._evalValue=0.0;

    _newState._blackPawns = 0b0000000011111111000000000000000000000000000000000000000000000000;
    _newState._whitePawns = 0b0000000000000000000000000000000000000000000000001111111100000000;
    _newState._blackRooks = 0b1000000100000000000000000000000000000000000000000000000000000000;
    _newState._blackKnights = 0b0100001000000000000000000000000000000000000000000000000000000000;
    _newState._blackBishops = 0b0010010000000000000000000000000000000000000000000000000000000000;
    _newState._blackKing = 0b0000100000000000000000000000000000000000000000000000000000000000;
    _newState._blackQueens = 0b0001000000000000000000000000000000000000000000000000000000000000;
    _newState._whiteRooks = 0b0000000000000000000000000000000000000000000000000000000010000001;
    _newState._whiteKnights = 0b0000000000000000000000000000000000000000000000000000000001000010;
    _newState._whiteBishops = 0b0000000000000000000000000000000000000000000000000000000000100100;
    _newState._whiteQueens = 0b0000000000000000000000000000000000000000000000000000000000010000;
    _newState._whiteKing = 0b0000000000000000000000000000000000000000000000000000000000001000;
    _newState._evalValue=0;


    _newState._occupancy =
        _newState._blackPawns ^ _newState._blackRooks ^
        _newState._blackKnights ^ _newState._blackBishops ^ _newState._blackQueens ^
        _newState._blackKing ^ _newState._whitePawns ^
        _newState._whiteRooks ^ _newState._whiteKnights ^
        _newState._whiteQueens ^ _newState._whiteKing ^
        _newState._whiteBishops;

    _newState._blackOccupancy =
        _newState._blackPawns ^ _newState._blackRooks ^
        _newState._blackKnights ^ _newState._blackBishops ^ _newState._blackQueens ^
        _newState._blackKing;

    _newState._whiteOccupancy = _newState._whitePawns ^
                                _newState._whiteRooks ^ _newState._whiteKnights ^
                                _newState._whiteQueens ^ _newState._whiteKing ^
                                _newState._whiteBishops;



    _newState._castlingAvailable = 0;
    _newState._castlingAvailable= 0b00001111;
    _newState._pieceToMove= 'w';
    _newState._zobristHash = generateZorbistHashFromAGameState(_newState);
    _newState._enpassantFile= '-';
    _newState._numberMoves=1;
    _newState._numberHalfMoves=0;
    return _newState;
}


void characterPuter(char _board[8][8], __uint64_t PIECE_NUMBER, char CHARACTER)
{
    int _counter = 0;
    __uint64_t _temp = PIECE_NUMBER;

    while (_temp)
    {
        int _mod = _temp % 256;
        int _secondCounter = 0;

        while (_mod)
        {
            if (_mod & 1)
            {
                _board[_counter][_secondCounter] = CHARACTER;
            }
            _mod >>= 1;
            _secondCounter++;
        }

        _temp >>= 8;
        _counter++;
    }
}

void piecePuter(Pieces _board[64] , __uint64_t PIECE_NUMBER , Pieces piece){
    int _counter = 0;
    __uint64_t _temp = PIECE_NUMBER;


    for(int i=0;i<64; i++){
        if(_temp & 1)
        {
            _board[i]=piece;
        }

        _temp >>= 1;
    }
    
}


// Zorbist Hashing implementation

ZorbistKeys generateZorbistNumbers(){
    __uint64_t seed = 0xCAFEBABEDEADBEEFULL;
    ZorbistKeys _curr; 

    for(int i=0;i<12; i++){
        for(int j=0 ;j<64; j++){
            _curr._zorbistPieces[i][j]=pseudoRandomNumberGenerator(&seed);
        }
    }

    _curr._zorbistSideToMove=pseudoRandomNumberGenerator(&seed);
    
    for(int i=0;i<16; i++){
        _curr._zorbistCastlingNums[i]=pseudoRandomNumberGenerator(&seed);
    }
    
    for(int i=0;i<8; i++){
        _curr._zobistFileNums[i]=pseudoRandomNumberGenerator(&seed);
    }


    return _curr; 
}

__uint64_t generateXORforPiece(Pieces PIECE , __uint64_t PIECE_BIT_MAP){
    __uint64_t _curr=0 ;

    while(PIECE_BIT_MAP){
        _curr^=_globalZorbistHashing._zorbistPieces[PIECE][__builtin_ctzll((PIECE_BIT_MAP ^ (PIECE_BIT_MAP & (PIECE_BIT_MAP-1))))];
        PIECE_BIT_MAP=PIECE_BIT_MAP & (PIECE_BIT_MAP-1);

    }


    return _curr;

}

__uint64_t generateZorbistHashFromAGameState(GameState GAME_STATE){
    __uint64_t _curr =0;

    
    _curr ^=    generateXORforPiece(WHITE_KING , GAME_STATE._whiteKing) ^ 
                generateXORforPiece(WHITE_BISHOP, GAME_STATE._whiteBishops) ^
                generateXORforPiece(WHITE_QUEEN , GAME_STATE._whiteQueens) ^
                generateXORforPiece(WHITE_KNIGHT , GAME_STATE._whiteKnights) ^
                generateXORforPiece(WHITE_ROOK , GAME_STATE._whiteRooks) ^
                generateXORforPiece(BLACK_KING  , GAME_STATE._blackKing) ^
                generateXORforPiece(BLACK_BISHOP, GAME_STATE._blackBishops) ^
                generateXORforPiece(BLACK_KNIGHT , GAME_STATE._blackKnights) ^
                generateXORforPiece(BLACK_QUEEN , GAME_STATE._blackQueens) ^
                generateXORforPiece(BLACK_PAWN, GAME_STATE._blackPawns) ^
                generateXORforPiece(WHITE_PAWN , GAME_STATE._whitePawns) ;

    _curr^=_globalZorbistHashing._zorbistCastlingNums[GAME_STATE._castlingAvailable & 0b00001111];


    if(GAME_STATE._enpassantFile != '-')
    {
        _curr^=_globalZorbistHashing._zobistFileNums[(GAME_STATE._enpassantFile - 'a')];
    }

    return _curr;
}