#pragma once;
#include "Common.h"


//function
ZorbistKeys generateZorbistNumbers();
__uint64_t generateZorbistHashFromAGameState(GameState GAME_STATE);


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



    __uint64_t *_prevStates;
    int _stateIndex;
    int _totalState;
    
    int  _numberHalfMoves;
    int _numberMoves;

    char* _castlingAvailable;
    char _pieceToMove;

    char _enpassantFile ;

} GameState;

typedef enum
{
    A1,
    A2,
    A3,
    A4,
    A5,
    A6,
    A7,
    A8,
    B1,
    B2,
    B3,
    B4,
    B5,
    B6,
    B7,
    B8,
    C1,
    C2,
    C3,
    C4,
    C5,
    C6,
    C7,
    C8,
    D1,
    D2,
    D3,
    D4,
    D5,
    D6,
    D7,
    D8,
    E1,
    E2,
    E3,
    E4,
    E5,
    E6,
    E7,
    E8,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    G1,
    G2,
    G3,
    G4,
    G5,
    G6,
    G7,
    G8,
    H1,
    H2,
    H3,
    H4,
    H5,
    H6,
    H7,
    H8
} Square;

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
    BLACK_PAWN
} Pieces;

typedef struct
{
    __uint64_t _zorbistPieces[12][64];
    __uint64_t _zorbistSideToMove;
    __uint64_t _zorbistCastlingNums[4];
    __uint64_t _zobistFileNums[8];
    
} ZorbistKeys ;

static ZorbistKeys _globalZorbistHashing={};


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


    _newState._prevStates = (__uint64_t *)malloc(sizeof(__uint64_t) * 100);
    _newState._stateIndex = 0;
    _newState._totalState = 100;
    _newState._castlingAvailable = (char*)malloc(sizeof(char) * 4 );
    _newState._castlingAvailable= "KQkq";
    _newState._pieceToMove= 'w';
    _newState._zobristHash = generateZorbistHashFromAGameState(_newState);
    _newState._enpassantFile= '-';
    _newState._numberMoves=1;
    _newState._numberHalfMoves=0;
    return _newState;
}

void insertStateIntoGameState(GameState GAME_STATE, __uint64_t VALUE)
{
    if (GAME_STATE._stateIndex < GAME_STATE._totalState / 2)
    {
        GAME_STATE._prevStates[GAME_STATE._stateIndex] = VALUE;
        GAME_STATE._stateIndex++;

        return;
    }
    __uint64_t *new_array = (__uint64_t *)malloc(sizeof(__uint64_t) * 2 * GAME_STATE._totalState);

    for (int i = 0; i <= GAME_STATE._stateIndex; i++)
    {
        new_array[i] = GAME_STATE._prevStates[i];
    }

    free(GAME_STATE._prevStates);
    GAME_STATE._prevStates = new_array;
}

GameState characterPuter(char _board[8][8], __uint64_t PIECE_NUMBER, char CHARACTER)
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

GameState printBoard(GameState GAME_STATE)
{
    char _board[8][8];

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            _board[i][j] = '.';
        }
    }

    // black pawns
    characterPuter(_board, GAME_STATE._blackPawns, 'p');
    // black rooks
    characterPuter(_board, GAME_STATE._blackRooks, 'r');
    // black bishops
    characterPuter(_board, GAME_STATE._blackBishops, 'b');
    // black knights
    characterPuter(_board, GAME_STATE._blackKnights, 'n');
    // black queens
    characterPuter(_board, GAME_STATE._blackQueens, 'q');
    // black king
    characterPuter(_board, GAME_STATE._blackKing, 'k');

    // white pawns
    characterPuter(_board, GAME_STATE._whitePawns, 'P');
    // white rooks
    characterPuter(_board, GAME_STATE._whiteRooks, 'R');
    // white bishops
    characterPuter(_board, GAME_STATE._whiteBishops, 'B');
    // white knights
    characterPuter(_board, GAME_STATE._whiteKnights, 'N');
    // white queens
    characterPuter(_board, GAME_STATE._whiteQueens, 'q');
    // white king
    characterPuter(_board, GAME_STATE._whiteKing, 'K');

    for (int i = 7; i >= 0; i--)
    {
        for (int j = 0; j < 8; j++)
        {
            printf("%c ", _board[i][j]);
        }
        printf("\n");
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
    
    for(int i=0;i<4; i++){
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
        _curr^=_globalZorbistHashing._zorbistPieces[PIECE][(log2((PIECE_BIT_MAP ^ (PIECE_BIT_MAP & (PIECE_BIT_MAP-1)))))];
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


    if(GAME_STATE._castlingAvailable[0]=='K')
    {
        _curr^=_globalZorbistHashing._zorbistCastlingNums[0];
    }
    if(GAME_STATE._castlingAvailable[1]=='Q')
    {
        _curr^=_globalZorbistHashing._zorbistCastlingNums[1];
    }
    if(GAME_STATE._castlingAvailable[2]=='k')
    {
        _curr^=_globalZorbistHashing._zorbistCastlingNums[2];
    }
    if(GAME_STATE._castlingAvailable[3]=='q')
    {
        _curr^=_globalZorbistHashing._zorbistCastlingNums[3];
    }

    if(GAME_STATE._enpassantFile != '-')
    {
        _curr^=_globalZorbistHashing._zobistFileNums[(GAME_STATE._enpassantFile - 'a')];
    }

    return _curr;
}