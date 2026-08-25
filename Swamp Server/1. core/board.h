#pragma once
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include "./multithread.h"
#include "./locks.h"

#ifndef __uint64_t
#define __uint64_t uint64_t
#endif
#ifndef __uint8_t
#define __uint8_t uint8_t
#endif


#define MAX_MOVE_COUNT 256
#define WHITE_KING_OCCUPANCY 0
#define WHITE_QUEEN_OCCUPANCY 1
#define WHITE_ROOK_OCCUPANCY 2
#define WHITE_BISHOP_OCCUPANCY 3
#define WHITE_KNIGHT_OCCUPANCY 4
#define WHITE_PAWN_OCCUPANCY 5
#define BLACK_KING_OCCUPANCY 6
#define BLACK_QUEEN_OCCUPANCY 7
#define BLACK_ROOK_OCCUPANCY 8
#define BLACK_BISHOP_OCCUPANCY 9
#define BLACK_KNIGHT_OCCUPANCY 10
#define BLACK_PAWN_OCCUPANCY 11
#define WHITE_OCCUPANCY 12
#define BLACK_OCCUPANCY 13
#define TOTAL_OCCUPANCY 14
#define ZORBIST_HASH 15
#define EVALUATION 16
#define NUMBER_HALF_MOVES 17
#define NUMBER_FULL_MOVES 18
#define CASTLING_AVAILABLE 19
#define SIDE 20
#define ENPASSANT_SQUARE 21
#define EMPTY_SQUARE 22
#define NO_PIECE 23
#define UCI_LINE_SIZE 4096


typedef enum
{
    WHITE_KING,
    WHITE_QUEEN,
    WHITE_ROOK,
    WHITE_BISHOP,
    WHITE_KNIGHT,
    WHITE_PAWN,
    BLACK_KING,
    BLACK_QUEEN,
    BLACK_ROOK,
    BLACK_BISHOP,
    BLACK_KNIGHT,
    BLACK_PAWN,
    ES,
    NP
} Pieces;

typedef enum
{
    A1,
    B1,
    C1,
    D1,
    E1,
    F1,
    G1,
    H1,
    A2,
    B2,
    C2,
    D2,
    E2,
    F2,
    G2,
    H2,
    A3,
    B3,
    C3,
    D3,
    E3,
    F3,
    G3,
    H3,
    A4,
    B4,
    C4,
    D4,
    E4,
    F4,
    G4,
    H4,
    A5,
    B5,
    C5,
    D5,
    E5,
    F5,
    G5,
    H5,
    A6,
    B6,
    C6,
    D6,
    E6,
    F6,
    G6,
    H6,
    A7,
    B7,
    C7,
    D7,
    E7,
    F7,
    G7,
    H7,
    A8,
    B8,
    C8,
    D8,
    E8,
    F8,
    G8,
    H8,
    NS,
    WE,
    BE
} Square;

typedef struct
{
    uint16_t moves[MAX_MOVE_COUNT];
    int index;

} MoveList;



typedef struct
{
    uint64_t _zorbistPieces[12][64];
    uint64_t _zorbistSideToMove;
    uint64_t _zorbistCastlingNums[16];
    uint64_t _zobistFileNums[8];

} ZorbistKeys;

extern uint64_t knightTable[64];
extern uint64_t whitePawnTable[64];
extern uint64_t whitePawnMovementTable[64];
extern uint64_t blackPawnTable[64];
extern uint64_t blackPawnMovementTable[64];
extern uint64_t kingTable[64];
extern uint64_t bishopAttacks[64][512];
extern uint64_t rookAttacks[64][4096];
extern uint64_t rookMask[64];
extern uint64_t bishopMask[64];
extern const __uint64_t pawnEnpassantMask[2];
extern int helperArr[13][2];

extern const uint64_t rookMagicNumbers[64];

extern const uint64_t bishopMagicNumbers[64];

extern const int bishopRelevantBits[64];

extern const int rookRelevantBits[64];



extern ZorbistKeys _globalZorbistHashing;

// ---------------------------------------
// ----------- GLOBAL ARRAY --------------
// ---------------------------------------


#define SET_BIT_AT(rank, file) \
    ((uint64_t)1ULL << (((rank) - 1) * 8 + (file)))

#define SET_BIT_PIECE(piece, rank, file) \
    do { \
        (piece) |= SET_BIT_AT((rank), (file)); \
        (file)++; \
    } while (0)

typedef enum
{
    PIECE_PLACEMENT,
    SIDE_TO_MOVE,
    CASTLING_ABILITY,
    ENPASSANT_TARGET,
    HALF_MOVE,
    FULL_MOVE,
    DONE
} FEN_STATE;

typedef struct FEN_STRING
{
    char piecePlacement[100];
    char sideTOMove;
    char castlingAbility[10];
    char enpassantTargetSquare[5];
    char halfmoveClock[10];
    char fullMoveClock[10];

} FEN_STRING;


typedef struct GAME_STATE_STRUCT{
    __uint64_t GAME_STATE[24];
    __uint8_t _chessBoard[64];
    __uint64_t _globalZorbistHashing;

} GAME_STATE_STRUCT;

typedef struct GAME_STATE_STRUCT_WITH_DEPTH{
    int depth;
    GAME_STATE_STRUCT state;
} GAME_STATE_STRUCT_WITH_DEPTH;



extern const uint8_t CASTLING_ARRAY[];
void printBoard();
void printPieceBitBoard(uint64_t bitboard, char piece);
void pieceInitializer();
void checkKingInCheckAndAddMove(int curr , int opp ,Square src , Square dest,int _promotion ,MoveList* _moveList );
uint64_t pseudoRandomNumberGenerator(uint64_t *seed);
ZorbistKeys generateZorbistNumbers();
__uint64_t generateXORforPiece(Pieces PIECE, uint64_t PIECE_BIT_MAP);
__uint64_t generateZorbistHashFromAGameState();
bool isSquareAttacked(Square square, int opp);
void generateMoveList(MoveList * _moves);
__uint64_t makeMove(uint16_t move);
__uint64_t perft(int depth );
__uint64_t perftBulk(int depth);
__uint64_t divide(int depth);
__uint64_t divideBulk(int depth );
__uint64_t divideBulkWithThread(int depth);
void piecePuter(uint64_t PIECE_NUMBER, Pieces piece );
void emptyInitializationHelper();
void initializeHelperFunc();
void initializer();
int initializeNewGameFromString(char *string );
void initializeNewGameFromStruct(GAME_STATE_STRUCT*);
void piecePuterWithBoard(uint64_t PIECE_NUMBER, Pieces piece ,uint8_t _chessBoard[64] );
GAME_STATE_STRUCT copyState();
void doMultiThreadPerft();



void pieceInitializer();
void generatePreCalculatedKingAttack();
void generatePreCalculateBlackPawnAttck();
void generatePreCalculatedWhitePawnAttack();
void generatePreCalculatedKnightAttack();
void initSliderAttack();
uint64_t set_occupancy(int index, int bits_in_mask, uint64_t attack_mask);
uint64_t rook_attacks_on_the_fly(int square, uint64_t block);
uint64_t bishop_attacks_on_the_fly(int square, uint64_t block);
void initMasks();
void generateBishopMask();
void generateRookMask();
