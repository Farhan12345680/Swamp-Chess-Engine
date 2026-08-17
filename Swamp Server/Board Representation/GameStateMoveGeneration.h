#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#ifndef __uint64_t
#define __uint64_t uint64_t
#endif
#ifndef __uint8_t
#define __uint8_t uint8_t
#endif

//-----------------------------------
//-----------------------------------
//---- DATA STRUCTURES & CONSTANTS---
//-----------------------------------
//-----------------------------------
#define MAX_MOVE_COUNT 256

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


static inline void checkKingInCheckAndAddMove(int curr , int opp
                                    ,Square src , Square dest,int _promotion
                                    ,MoveList*    _moveList);
int helperArr[13][2];




const uint8_t CASTLING_ARRAY[] = {
    [0 ... 63] = 0b00001111,
    [A1] = 0b00001011,
    [E1] = 0b00000011,
    [H1] = 0b00000111,
    [A8] = 0b00001110,
    [E8] = 0b00001100,
    [H8] = 0b00001101};

typedef struct
{
    uint64_t _zorbistPieces[12][64];
    uint64_t _zorbistSideToMove;
    uint64_t _zorbistCastlingNums[16];
    uint64_t _zobistFileNums[8];

} ZorbistKeys;

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




// ----------------------------------------
// ------- function forward Decleration ---
//-----------------------------------------

static bool isSquareAttacked(Square square, int opp);

// ----------------------------------------
// ------------- GLOBAL VARIABLE ----------
// ----------------------------------------

uint64_t GAME_STATE[24];
Pieces _chessBoard[64];
ZorbistKeys _globalZorbistHashing;
uint64_t knightTable[64];
uint64_t whitePawnTable[64];
uint64_t whitePawnMovementTable[64];
uint64_t blackPawnTable[64];
uint64_t blackPawnMovementTable[64];
uint64_t kingTable[64];
uint64_t bishopAttacks[64][512];
uint64_t rookAttacks[64][4096];
uint64_t rookMask[64];
uint64_t bishopMask[64];
const __uint64_t pawnEnpassantMask[2]={
                                 0b0000000000000000000000001111111100000000000000000000000000000000,
                                 0b0000000000000000000000000000000011111111000000000000000000000000};


// -----------------------------------------
// -------------- HASH GENERATOR -----------
// -----------------------------------------

uint64_t pseudoRandomNumberGenerator(uint64_t *seed)
{
    uint64_t _z = (*seed += 0x9E3779B97F4A7C15ULL);

    _z = (_z ^ (_z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    _z = (_z ^ (_z >> 27)) * 0x94D049BB133111EBULL;

    return _z ^ (_z >> 31);
}

ZorbistKeys generateZorbistNumbers()
{
    uint64_t seed = 0xCAFEBABEDEADBEEFULL;
    ZorbistKeys _curr;

    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            _curr._zorbistPieces[i][j] = pseudoRandomNumberGenerator(&seed);
        }
    }

    _curr._zorbistSideToMove = pseudoRandomNumberGenerator(&seed);

    for (int i = 0; i < 16; i++)
    {
        _curr._zorbistCastlingNums[i] = pseudoRandomNumberGenerator(&seed);
    }

    for (int i = 0; i < 8; i++)
    {
        _curr._zobistFileNums[i] = pseudoRandomNumberGenerator(&seed);
    }
    return _curr;
}

uint64_t generateXORforPiece(Pieces PIECE, uint64_t PIECE_BIT_MAP)
{
    uint64_t _curr = 0;

    while (PIECE_BIT_MAP)
    {
        _curr ^= _globalZorbistHashing._zorbistPieces[PIECE][__builtin_ctzll(PIECE_BIT_MAP)];
        PIECE_BIT_MAP = PIECE_BIT_MAP & (PIECE_BIT_MAP - 1);
    }

    return _curr;
}

uint64_t generateZorbistHashFromAGameState()
{
    uint64_t _curr = 0;

    _curr ^= generateXORforPiece(WHITE_KING, GAME_STATE[WHITE_KING_OCCUPANCY]) ^
             generateXORforPiece(WHITE_PAWN, GAME_STATE[WHITE_PAWN_OCCUPANCY]) ^
             generateXORforPiece(WHITE_BISHOP, GAME_STATE[WHITE_BISHOP_OCCUPANCY]) ^
             generateXORforPiece(WHITE_QUEEN, GAME_STATE[WHITE_QUEEN_OCCUPANCY]) ^
             generateXORforPiece(WHITE_KNIGHT, GAME_STATE[WHITE_KNIGHT_OCCUPANCY]) ^
             generateXORforPiece(WHITE_ROOK, GAME_STATE[WHITE_ROOK_OCCUPANCY]) ^
             generateXORforPiece(BLACK_KING, GAME_STATE[BLACK_KING_OCCUPANCY]) ^
             generateXORforPiece(BLACK_BISHOP, GAME_STATE[BLACK_BISHOP_OCCUPANCY]) ^
             generateXORforPiece(BLACK_KNIGHT, GAME_STATE[BLACK_KNIGHT_OCCUPANCY]) ^
             generateXORforPiece(BLACK_QUEEN, GAME_STATE[BLACK_QUEEN_OCCUPANCY]) ^
             generateXORforPiece(BLACK_PAWN, GAME_STATE[BLACK_PAWN_OCCUPANCY]) ^
             generateXORforPiece(BLACK_ROOK ,GAME_STATE[BLACK_ROOK_OCCUPANCY]);

    _curr ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];

    if (GAME_STATE[ENPASSANT_SQUARE] != NS)
    {
        _curr ^= _globalZorbistHashing._zobistFileNums[(GAME_STATE[ENPASSANT_SQUARE] % 8)];
    }

    return _curr;
}

// ----------------------------------------
// ----------- DEBUG PRINTER --------------
// ----------------------------------------

void printPieceBitBoard(uint64_t bitboard, char piece)
{
    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;

            if (bitboard & (1ULL << square))
                printf("%c ", piece);
            else
                printf(". ");
        }

        printf("\n");
    }
}

void printBoard(void)
{
    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int sq = rank * 8 + file;

            switch (_chessBoard[sq])
            {
            case BLACK_ROOK:
                printf("r ");
                break;
            case BLACK_KNIGHT:
                printf("n ");
                break;
            case BLACK_BISHOP:
                printf("b ");
                break;
            case BLACK_QUEEN:
                printf("q ");
                break;
            case BLACK_KING:
                printf("k ");
                break;
            case BLACK_PAWN:
                printf("p ");
                break;

            case WHITE_ROOK:
                printf("R ");
                break;
            case WHITE_KNIGHT:
                printf("N ");
                break;
            case WHITE_BISHOP:
                printf("B ");
                break;
            case WHITE_QUEEN:
                printf("Q ");
                break;
            case WHITE_KING:
                printf("K ");
                break;
            case WHITE_PAWN:
                printf("P ");
                break;

            default:
                printf(". ");
                break;
            }
        }

        printf("\n");
    }

    printf("castling available %d%d%d%d\n" ,(GAME_STATE[CASTLING_AVAILABLE]&(1<<3)),
                                            (GAME_STATE[CASTLING_AVAILABLE]&(1<<2)),
                                            (GAME_STATE[CASTLING_AVAILABLE]&(1<<1)),
                                            (GAME_STATE[CASTLING_AVAILABLE]&(1<<0)));

    printf("Side to Move %c\n", (GAME_STATE[SIDE] == 6)?'b':'w');
    if(GAME_STATE[ENPASSANT_SQUARE]==ES){
        printf("Enpassant File ES\n");
    }else{
        printf("Enpassant File %c%c\n", ((char)(GAME_STATE[ENPASSANT_SQUARE]%8)+'a'), ((char)(GAME_STATE[ENPASSANT_SQUARE]/8)+'1'));
    }

    printf("Full Move %d\n",(int)GAME_STATE[NUMBER_FULL_MOVES]);
    printf("HALF Move %d\n",(int)GAME_STATE[NUMBER_HALF_MOVES]);

}

// ------------------------------------------
// ------------- PIECE MOVEMENT ------------
// ------------------------------------------

const uint64_t rookMagicNumbers[64] = {
    0x8a80104000800020ULL,
    0x140002000100040ULL,
    0x2801880a0017001ULL,
    0x100081001000420ULL,
    0x200020010080420ULL,
    0x3001c0002010008ULL,
    0x8480008002000100ULL,
    0x2080088004402900ULL,
    0x800098204000ULL,
    0x2024401000200040ULL,
    0x100802000801000ULL,
    0x120800800801000ULL,
    0x208808088000400ULL,
    0x2802200800400ULL,
    0x2200800100020080ULL,
    0x801000060821100ULL,
    0x80044006422000ULL,
    0x100808020004000ULL,
    0x12108a0010204200ULL,
    0x140848010000802ULL,
    0x481828014002800ULL,
    0x8094004002004100ULL,
    0x4010040010010802ULL,
    0x20008806104ULL,
    0x100400080208000ULL,
    0x2040002120081000ULL,
    0x21200680100081ULL,
    0x20100080080080ULL,
    0x2000a00200410ULL,
    0x20080800400ULL,
    0x80088400100102ULL,
    0x80004600042881ULL,
    0x4040008040800020ULL,
    0x440003000200801ULL,
    0x4200011004500ULL,
    0x188020010100100ULL,
    0x14800401802800ULL,
    0x2080040080800200ULL,
    0x124080204001001ULL,
    0x200046502000484ULL,
    0x480400080088020ULL,
    0x1000422010034000ULL,
    0x30200100110040ULL,
    0x100021010009ULL,
    0x2002080100110004ULL,
    0x202008004008002ULL,
    0x20020004010100ULL,
    0x2048440040820001ULL,
    0x101002200408200ULL,
    0x40802000401080ULL,
    0x4008142004410100ULL,
    0x2060820c0120200ULL,
    0x1001004080100ULL,
    0x20c020080040080ULL,
    0x2935610830022400ULL,
    0x44440041009200ULL,
    0x280001040802101ULL,
    0x2100190040002085ULL,
    0x80c0084100102001ULL,
    0x4024081001000421ULL,
    0x20030a0244872ULL,
    0x12001008414402ULL,
    0x2006104900a0804ULL,
    0x1004081002402ULL};

const uint64_t bishopMagicNumbers[64] = {
    0x40040844404084ULL,
    0x2004208a004208ULL,
    0x10190041080202ULL,
    0x108060845042010ULL,
    0x581104180800210ULL,
    0x2112080446200010ULL,
    0x1080820820060210ULL,
    0x3c0808410220200ULL,
    0x4050404440404ULL,
    0x21001420088ULL,
    0x24d0080801082102ULL,
    0x1020a0a020400ULL,
    0x40308200402ULL,
    0x4011002100800ULL,
    0x401484104104005ULL,
    0x801010402020200ULL,
    0x400210c3880100ULL,
    0x404022024108200ULL,
    0x810018200204102ULL,
    0x4002801a02003ULL,
    0x85040820080400ULL,
    0x810102c808880400ULL,
    0xe900410884800ULL,
    0x8002020480840102ULL,
    0x220200865090201ULL,
    0x2010100a02021202ULL,
    0x152048408022401ULL,
    0x20080002081110ULL,
    0x4001001021004000ULL,
    0x800040400a011002ULL,
    0xe4004081011002ULL,
    0x1c004001012080ULL,
    0x8004200962a00220ULL,
    0x8422100208500202ULL,
    0x2000402200300c08ULL,
    0x8646020080080080ULL,
    0x80020a0200100808ULL,
    0x2010004880111000ULL,
    0x623000a080011400ULL,
    0x42008c0340209202ULL,
    0x209188240001000ULL,
    0x400408a884001800ULL,
    0x110400a6080400ULL,
    0x1840060a44020800ULL,
    0x90080104000041ULL,
    0x201011000808101ULL,
    0x1a2208080504f080ULL,
    0x8012020600211212ULL,
    0x500861011240000ULL,
    0x180806108200800ULL,
    0x4000020e01040044ULL,
    0x300000261044000aULL,
    0x802241102020002ULL,
    0x20906061210001ULL,
    0x5a84841004010310ULL,
    0x4010801011c04ULL,
    0xa010109502200ULL,
    0x4a02012000ULL,
    0x500201010098b028ULL,
    0x8040002811040900ULL,
    0x28000010020204ULL,
    0x6000020202d0240ULL,
    0x8918844842082200ULL,
    0x4010011029020020ULL

};

const int bishopRelevantBits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6};

const int rookRelevantBits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12};

static void generateRookMask()
{
    for (int i = 1; i < 9; i++)
    {
        for (int j = 1; j < 9; j++)
        {
            int _idx = (i - 1) * 8 + (j - 1);
            uint64_t _point = 1ULL << _idx;
            uint64_t _mask = 0;

            for (int k = i + 1; k < 8; k++)
            {
                _mask |= _point << (8 * (abs(k - i)));
            }
            for (int k = i - 1; k > 1; k--)
            {
                _mask |= _point >> (8 * (abs(k - i)));
            }
            for (int k = j + 1; k < 8; k++)
            {
                _mask |= _point << ((abs(k - j)));
            }
            for (int k = j - 1; k > 1; k--)
            {
                _mask |= _point >> ((abs(k - j)));
            }

            rookMask[_idx] = _mask;
        }
    }
}

static void generateBishopMask()
{
    for (int row = 1; row < 9; row++)
    {
        for (int column = 1; column < 9; column++)
        {
            int _idx = (row - 1) * 8 + (column - 1);
            uint64_t _point = 1ULL << _idx;
            uint64_t _mask = 0;

            for (int k = row + 1, l = column - 1; k < 8 && l > 1; k++, l--)
            {
                _mask |= _point << (7 * (abs(k - row)));
            }
            for (int k = row + 1, l = column + 1; k < 8 && l < 8; k++, l++)
            {
                _mask |= _point << (9 * (abs(k - row)));
            }
            for (int k = row - 1, l = column - 1; k > 1 && l > 1; k--, l--)
            {
                _mask |= _point >> (9 * (abs(k - row)));
            }
            for (int k = row - 1, l = column + 1; k > 1 && l < 8; k--, l++)
            {
                _mask |= _point >> (7 * (abs(k - row)));
            }

            bishopMask[_idx] = _mask;
        }
    }
}

static void initMasks()
{
    generateRookMask();
    generateBishopMask();
}

static uint64_t bishop_attacks_on_the_fly(int square, uint64_t block)
{
    uint64_t attacks = 0ULL;

    int r, f;

    int tr = square / 8;
    int tf = square % 8;

    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block)
            break;
    }

    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block)
            break;
    }

    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block)
            break;
    }

    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block)
            break;
    }

    return attacks;
}

static uint64_t rook_attacks_on_the_fly(int square, uint64_t block)
{
    uint64_t attacks = 0ULL;

    int r, f;

    int tr = square / 8;
    int tf = square % 8;

    for (r = tr + 1; r <= 7; r++)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & block)
            break;
    }

    for (r = tr - 1; r >= 0; r--)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & block)
            break;
    }

    for (f = tf + 1; f <= 7; f++)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & block)
            break;
    }

    for (f = tf - 1; f >= 0; f--)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & block)
            break;
    }

    return attacks;
}

static uint64_t set_occupancy(int index, int bits_in_mask, uint64_t attack_mask)
{
    uint64_t occupancy = 0ULL;

    for (int count = 0; count < bits_in_mask; count++)
    {
        int square = __builtin_ctzll(attack_mask);

        attack_mask &= (attack_mask - 1);

        if (index & (1 << count))
            occupancy |= (1ULL << square);
    }

    return occupancy;
}

static void initSliderAttack()
{
    initMasks();
    {
        for (int square = 0; square < 64; square++)
        {

            uint64_t attack_mask = bishopMask[square];

            int relevant_bits_count = __builtin_popcountll(attack_mask);

            int occupancy_indicies = (1 << relevant_bits_count);

            for (int index = 0; index < occupancy_indicies; index++)
            {

                uint64_t occupancy = set_occupancy(index, relevant_bits_count, attack_mask);

                int magic_index = (occupancy * bishopMagicNumbers[square]) >> (64 - bishopRelevantBits[square]);

                bishopAttacks[square][magic_index] = bishop_attacks_on_the_fly(square, occupancy);
            }
        }
    }

    {
        for (int square = 0; square < 64; square++)
        {

            uint64_t attack_mask = rookMask[square];

            int relevant_bits_count = __builtin_popcountll(attack_mask);

            int occupancy_indicies = (1 << relevant_bits_count);

            for (int index = 0; index < occupancy_indicies; index++)
            {

                {
                    uint64_t occupancy = set_occupancy(index, relevant_bits_count, attack_mask);

                    int magic_index = (occupancy * rookMagicNumbers[square]) >> (64 - rookRelevantBits[square]);

                    rookAttacks[square][magic_index] = rook_attacks_on_the_fly(square, occupancy);
                }
            }
        }
    }
}

static void generatePreCalculatedKnightAttack()
{
    for (int i = 1; i <= 8; i++)
    {
        for (int j = 1; j <= 8; j++)
        {
            int _idx = (i - 1) * 8 + ((j - 1));
            knightTable[_idx] = 0;
            uint64_t _curr = 1ULL << _idx;

            if ((i - 2) > 0 && (j - 1) > 0)
            {
                knightTable[_idx] |= (_curr >> 17);
            }
            if ((i - 2) > 0 && (j + 1) < 9)
            {
                knightTable[_idx] |= (_curr >> 15);
            }
            if ((i + 2) < 9 && (j - 1) > 0)
            {
                knightTable[_idx] |= (_curr << 15);
            }
            if ((i + 2) < 9 && (j + 1) < 9)
            {
                knightTable[_idx] |= (_curr << 17);
            }
            if ((j - 2) > 0 && (i - 1) > 0)
            {
                knightTable[_idx] |= (_curr >> 10);
            }
            if ((j - 2) > 0 && (i + 1) < 9)
            {
                knightTable[_idx] |= (_curr << 6);
            }
            if ((j + 2) < 9 && (i - 1) > 0)
            {
                knightTable[_idx] |= (_curr >> 6);
            }
            if ((j + 2) < 9 && (i + 1) < 9)
            {
                knightTable[_idx] |= (_curr << 10);
            }
        }
    }
}

static void generatePreCalculatedWhitePawnAttack()
{
    for (int i = 0; i <= 7; i++)
    {
        for (int j = 0; j <= 7; j++)
        {
            int _idx = (i) * 8 + ((j));
            whitePawnTable[_idx] = 0;
            uint64_t _curr = 1ULL << _idx;

            if ((j) > 0)
            {
                whitePawnTable[_idx] |= (_curr << 7);
            }
            if ((j) < 7)
            {
                whitePawnTable[_idx] |= (_curr << 9);
            }
        }
    }
}

static void generatePreCalculateBlackPawnAttck()
{
    for (int i = 0; i <= 7; i++)
    {
        for (int j = 0; j <= 7; j++)
        {
            int _idx = (i) * 8 + ((j));
            blackPawnTable[_idx] = 0;
            uint64_t _curr = 1ULL << _idx;

            if (j > 0)
                blackPawnTable[_idx] |= _curr >> 9;

            if (j < 7)
                blackPawnTable[_idx] |= _curr >> 7;
        }
    }
}

static void generatePreCalculatedKingAttack()
{
    for (int i = 1; i <= 8; i++)
    {
        for (int j = 1; j <= 8; j++)
        {
            int _idx = (i - 1) * 8 + ((j - 1));

            kingTable[_idx] = 0;
            uint64_t _curr = 1ULL << _idx;

            if ((i - 1) > 0 && (j - 1) > 0)
            {
                kingTable[_idx] |= (_curr >> 9);
            }
            if ((i - 1) > 0 && (j + 1) < 9)
            {
                kingTable[_idx] |= (_curr >> 7);
            }
            if ((i - 1) > 0)
            {
                kingTable[_idx] |= (_curr >> 8);
            }
            if ((i + 1) < 9 && (j - 1) > 0)
            {
                kingTable[_idx] |= (_curr << 7);
            }
            if ((i + 1) < 9 && (j + 1) < 9)
            {
                kingTable[_idx] |= (_curr << 9);
            }
            if ((i + 1) < 9)
            {
                kingTable[_idx] |= (_curr << 8);
            }
            if ((j - 1) > 0)
            {
                kingTable[_idx] |= (_curr >> 1);
            }
            if ((j + 1) < 9)
            {
                kingTable[_idx] |= (_curr << 1);
            }
        }
    }
}

void pieceInitializer()
{
    helperArr[ES][0]=NO_PIECE;
    helperArr[ES][1]=NO_PIECE;
    helperArr[BLACK_PAWN][0]=BLACK_OCCUPANCY;
    helperArr[BLACK_PAWN][1]=BLACK_PAWN_OCCUPANCY;
    helperArr[BLACK_KNIGHT][0]=BLACK_OCCUPANCY;
    helperArr[BLACK_KNIGHT][1]=BLACK_KNIGHT_OCCUPANCY;
    helperArr[BLACK_BISHOP][0]=BLACK_OCCUPANCY;
    helperArr[BLACK_BISHOP][1]=BLACK_BISHOP_OCCUPANCY;
    helperArr[BLACK_ROOK][0]=BLACK_OCCUPANCY;
    helperArr[BLACK_ROOK][1]=BLACK_ROOK_OCCUPANCY;
    helperArr[BLACK_QUEEN][0]=BLACK_OCCUPANCY;
    helperArr[BLACK_QUEEN][1]=BLACK_QUEEN_OCCUPANCY;
    helperArr[BLACK_KING][0]=BLACK_OCCUPANCY;
    helperArr[BLACK_KING][1]=BLACK_KING_OCCUPANCY;
    helperArr[WHITE_PAWN][0]=WHITE_OCCUPANCY;
    helperArr[WHITE_PAWN][1]=WHITE_PAWN_OCCUPANCY;
    helperArr[WHITE_KNIGHT][0]=WHITE_OCCUPANCY;
    helperArr[WHITE_KNIGHT][1]=WHITE_KNIGHT_OCCUPANCY;
    helperArr[WHITE_BISHOP][0]=WHITE_OCCUPANCY;
    helperArr[WHITE_BISHOP][1]=WHITE_BISHOP_OCCUPANCY;
    helperArr[WHITE_ROOK][0]=WHITE_OCCUPANCY;
    helperArr[WHITE_ROOK][1]=WHITE_ROOK_OCCUPANCY;
    helperArr[WHITE_QUEEN][0]=WHITE_OCCUPANCY;
    helperArr[WHITE_QUEEN][1]=WHITE_QUEEN_OCCUPANCY;
    helperArr[WHITE_KING][0]=WHITE_OCCUPANCY;
    helperArr[WHITE_KING][1]=WHITE_KING_OCCUPANCY;
    generateRookMask();
    generateBishopMask();

    initSliderAttack();
    generatePreCalculatedKnightAttack();
    generatePreCalculatedWhitePawnAttack();
    generatePreCalculateBlackPawnAttck();
    generatePreCalculatedKingAttack();
}

static inline uint64_t getBishopAttack(Square square, uint64_t occupancy, uint64_t sameSideOccupancy)
{
    occupancy &= bishopMask[square];
    uint64_t _idx = (occupancy * bishopMagicNumbers[square]) >> (64 - bishopRelevantBits[square]);
    return bishopAttacks[square][_idx] & (~sameSideOccupancy);
}

static inline uint64_t getRookAttack(Square square, uint64_t occupancy, uint64_t sameSideOccupancy)
{
    occupancy &= rookMask[square];
    uint64_t _idx = (occupancy * rookMagicNumbers[square]) >> (64 - rookRelevantBits[square]);
    return rookAttacks[square][_idx] & (~sameSideOccupancy);
}

static inline uint64_t getQueenAttack(Square square, uint64_t occupancy, uint64_t sameSideOccupancy)
{
    return getBishopAttack(square, occupancy, sameSideOccupancy) | getRookAttack(square, occupancy, sameSideOccupancy);
}

static inline uint64_t getKnightAttack(Square square, uint64_t occupancy, uint64_t sameSideOccupancy)
{
    return knightTable[square] & ~(sameSideOccupancy);
}

static inline bool isTheKingInCheck(int curr, int opp)
{
    uint64_t occ = GAME_STATE[TOTAL_OCCUPANCY];
    uint64_t ownOcc = GAME_STATE[(curr >> 2) + WHITE_OCCUPANCY];

    Square kingSq = (Square)__builtin_ctzll(GAME_STATE[GAME_STATE[SIDE]]);
    // if(GAME_STATE[ENPASSANT_SQUARE] != ES){
    //     printBoard();
    //     assert(ENPASSANT_SQUARE== ES );
    // }
    // if(kingSq == ES){
    //     printBoard();
    //     assert(kingSq!= ES );
    // }
    if (knightTable[kingSq] & GAME_STATE[opp + 4])
        return true;

    if (opp == WHITE_KING_OCCUPANCY)
    {
        if (blackPawnTable[kingSq] & GAME_STATE[WHITE_PAWN_OCCUPANCY])
            return true;
    }
    else
    {

        if (whitePawnTable[kingSq] & GAME_STATE[BLACK_PAWN_OCCUPANCY])
            return true;
    }


    if (kingTable[kingSq] & GAME_STATE[opp])
        return true;


    if (getBishopAttack(kingSq, occ, ownOcc) &
        (GAME_STATE[opp + 3] | GAME_STATE[opp + 1]))
        return true;


    if (getRookAttack(kingSq, occ, ownOcc) &
        (GAME_STATE[opp + 2] | GAME_STATE[opp + 1]))
        return true;

    return false;
}

static inline bool isTheKingInDoubleCheck(int curr, int opp)
{
    int counter=0;
    uint64_t occ = GAME_STATE[TOTAL_OCCUPANCY];
    uint64_t ownOcc = GAME_STATE[(curr >> 2) + WHITE_OCCUPANCY];

    Square kingSq = (Square)__builtin_ctzll(GAME_STATE[curr]);

    if (knightTable[kingSq] & GAME_STATE[opp + 4])
        counter+=1;

    if (opp == WHITE_KING_OCCUPANCY)
    {
        if (counter!=2 && blackPawnTable[kingSq] & GAME_STATE[WHITE_PAWN_OCCUPANCY])
            counter+=1;
    }
    else
    {

        if (counter!=2 && whitePawnTable[kingSq] & GAME_STATE[BLACK_PAWN_OCCUPANCY])
            counter+=1;
    }


    if ( counter!=2 && (kingTable[kingSq] & GAME_STATE[opp]))
        counter+=1;


    if (counter!=2 && getBishopAttack(kingSq, occ, ownOcc) &
        (GAME_STATE[opp + 3] | GAME_STATE[opp + 1]))
            counter+=1;


    if ( counter!=2 && getRookAttack(kingSq, occ, ownOcc) &
        (GAME_STATE[opp + 2] | GAME_STATE[opp + 1]))
        counter+=1;

    return counter == 2;
}

static inline uint64_t getKingAttackAndMovement(Square square, uint64_t sameSideOccupancy, uint64_t otherSideAttackTable)
{
    return kingTable[square] & ~(sameSideOccupancy) & ~(otherSideAttackTable);
}

static inline uint64_t getWhitePawnAttack(Square square, uint64_t occupancy, uint64_t sameSideOccupancy)
{
    return (whitePawnTable[square] & ~sameSideOccupancy) & occupancy;
}

static inline uint64_t getBlackPawnAttack(Square square, uint64_t occupancy, uint64_t sameSideOccupancy)
{
    return (blackPawnTable[square] & ~sameSideOccupancy) & occupancy;
}


// ----------------------------------------------------
// ------------------- MOVE GENERATION ----------------
// ----------------------------------------------------

// __uint64_t doMove(
//     uint8_t _src, uint8_t _dest, uint8_t _promotion,
//     uint8_t _colorOccupancySRC, uint8_t _pieceOccupancySRC,
//     uint8_t _colorOccupancyDEST, uint8_t _pieceOccupancyDEST,
//     Square EN)
// {
//     __uint64_t result = 0;

//     Pieces srcPiece  = _chessBoard[_src];
//     Pieces destPiece = _chessBoard[_dest];


//     result |= ((__uint64_t)(uint8_t)srcPiece)  << 40;
//     result |= ((__uint64_t)(uint8_t)destPiece) << 32;
//     result |= ((__uint64_t)_src)  << 16;
//     result |= ((__uint64_t)_dest) << 8;

//     uint64_t mask = 1ULL << _dest;
//     uint64_t _srcMask = 1ULL << _src;
//     uint64_t _gMask = mask | _srcMask;

//     GAME_STATE[_colorOccupancySRC] ^= _gMask;
//     GAME_STATE[_pieceOccupancySRC] ^= _gMask;

//     GAME_STATE[TOTAL_OCCUPANCY] ^= _srcMask;
//     GAME_STATE[TOTAL_OCCUPANCY] |= mask;

//     if (_colorOccupancyDEST != NO_PIECE &&
//         _pieceOccupancyDEST != NO_PIECE)
//     {
//         GAME_STATE[_colorOccupancyDEST] ^= mask;
//         GAME_STATE[_pieceOccupancyDEST] ^= mask;
//     }

//     GAME_STATE[NUMBER_FULL_MOVES] += GAME_STATE[SIDE] >> 2;

//     GAME_STATE[ZORBIST_HASH] ^=
//         _globalZorbistHashing
//         ._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];

//     GAME_STATE[CASTLING_AVAILABLE] =
//         GAME_STATE[CASTLING_AVAILABLE] & CASTLING_ARRAY[_src];

//     GAME_STATE[ZORBIST_HASH] ^=
//         _globalZorbistHashing
//         ._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];

//     switch (_chessBoard[_dest])
//     {
//         case ES:
//             GAME_STATE[NUMBER_HALF_MOVES] += 1;

//             if (_chessBoard[_src] == WHITE_PAWN ||
//                 _chessBoard[_src] == BLACK_PAWN)
//             {
//                 GAME_STATE[NUMBER_HALF_MOVES] = 0;
//             }
//             break;

//         default:
//             GAME_STATE[NUMBER_HALF_MOVES] = 0;

//             GAME_STATE[ZORBIST_HASH] ^=
//                 _globalZorbistHashing
//                 ._zorbistPieces[_chessBoard[_dest]][_dest];

//             GAME_STATE[ZORBIST_HASH] ^=
//                 _globalZorbistHashing
//                 ._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];

//             GAME_STATE[CASTLING_AVAILABLE] =
//                 GAME_STATE[CASTLING_AVAILABLE] & CASTLING_ARRAY[_dest];

//             GAME_STATE[ZORBIST_HASH] ^=
//                 _globalZorbistHashing
//                 ._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];
//     }

//     Pieces srcp = _chessBoard[_src];

//     _chessBoard[_dest] = _chessBoard[_src];
//     _chessBoard[_src] = ES;

//     GAME_STATE[ZORBIST_HASH] ^=
//         _globalZorbistHashing._zorbistSideToMove;

//     GAME_STATE[ZORBIST_HASH] ^=
//         _globalZorbistHashing
//         ._zorbistPieces[_chessBoard[_dest]][_dest];

//     GAME_STATE[ZORBIST_HASH] ^=
//         _globalZorbistHashing
//         ._zorbistPieces[srcp][_src];

//     GAME_STATE[ENPASSANT_SQUARE] = EN;

//     if (_promotion)
//     {
//         int pawn = GAME_STATE[SIDE] + 5;
//         int promotionSquare = GAME_STATE[SIDE] + _promotion;

//         GAME_STATE[pawn] ^= mask;

//         GAME_STATE[ZORBIST_HASH] ^=
//             _globalZorbistHashing
//             ._zorbistPieces[pawn][_dest];

//         _chessBoard[_dest] = (Pieces)promotionSquare;

//         GAME_STATE[promotionSquare] ^= mask;

//         GAME_STATE[ZORBIST_HASH] ^=
//             _globalZorbistHashing
//             ._zorbistPieces[_chessBoard[_dest]][_dest];
//     }

//     GAME_STATE[SIDE] ^= 6;

//     return result;
// }

static inline void doMove(
    uint8_t _src, uint8_t _dest, uint8_t _promotion,
    uint8_t _colorOccupancySRC, uint8_t _pieceOccupancySRC,
    uint8_t _colorOccupancyDEST, uint8_t _pieceOccupancyDEST,MoveList*    _moveList)
{

    uint64_t mask = 1ULL << _dest;
    uint64_t _srcMask = 1ULL << _src;
    uint64_t _gMask = mask | _srcMask;

    GAME_STATE[_colorOccupancySRC] ^= _gMask;
    GAME_STATE[_pieceOccupancySRC] ^= _gMask;
    GAME_STATE[_colorOccupancyDEST] ^= mask;
    GAME_STATE[_pieceOccupancyDEST] ^= mask;
    GAME_STATE[TOTAL_OCCUPANCY] = GAME_STATE[BLACK_OCCUPANCY] | GAME_STATE[WHITE_OCCUPANCY];


    checkKingInCheckAndAddMove(GAME_STATE[SIDE] , GAME_STATE[SIDE]^6 ,(Square)_src ,(Square) _dest ,_promotion,_moveList);


    GAME_STATE[_colorOccupancySRC] ^= _gMask;
    GAME_STATE[_pieceOccupancySRC] ^= _gMask;
    GAME_STATE[_colorOccupancyDEST] ^= mask;
    GAME_STATE[_pieceOccupancyDEST] ^= mask;
    GAME_STATE[TOTAL_OCCUPANCY] = GAME_STATE[BLACK_OCCUPANCY] | GAME_STATE[WHITE_OCCUPANCY];


}


// void doCastle(uint8_t _castleSide)
// {
//     GAME_STATE[NUMBER_HALF_MOVES] += 1;


//     switch (_castleSide)
//     {
//     case 0b00001000:
//         GAME_STATE[WHITE_OCCUPANCY] ^= GAME_STATE[WHITE_KING_OCCUPANCY] | (1ULL << H1);
//         GAME_STATE[TOTAL_OCCUPANCY] ^= GAME_STATE[WHITE_KING_OCCUPANCY] | (1ULL << H1);

//         GAME_STATE[WHITE_OCCUPANCY] |= (1ULL << G1) | (1ULL << F1);

//         GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][E1];
//         GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][G1];
//         GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[WHITE_ROOK][H1];
//         GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[WHITE_ROOK][F1];

//         GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= 0b10100000;
//         GAME_STATE[WHITE_KING_OCCUPANCY] = (1ULL << G1);
//         GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];
//         GAME_STATE[CASTLING_AVAILABLE] &= 0b00000011;
//         GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];
//         GAME_STATE[TOTAL_OCCUPANCY] |=
//             (1ULL << G1) | (1ULL << F1);
//         _chessBoard[H1] = ES;
//         _chessBoard[E1] = ES;
//         _chessBoard[G1] = WHITE_KING;
//         _chessBoard[F1] = WHITE_ROOK;
//         GAME_STATE[SIDE] ^= 6;

//         break;

//     case 0b00000100:
//         GAME_STATE[WHITE_OCCUPANCY] ^= GAME_STATE[WHITE_KING_OCCUPANCY] | (1ULL << A1);
//         GAME_STATE[TOTAL_OCCUPANCY] ^= GAME_STATE[WHITE_KING_OCCUPANCY] | (1ULL << A1);

//         GAME_STATE[WHITE_OCCUPANCY] |= (1ULL << C1) | (1ULL << D1);

//         GAME_STATE[TOTAL_OCCUPANCY] |= (1ULL << C1) | (1ULL << D1);

//         GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= 0b00001001;
//         GAME_STATE[WHITE_KING_OCCUPANCY] = (1ULL << C1);
//         GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][E1];
//         GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][C1];
//         GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[WHITE_ROOK][A1];
//         GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[WHITE_ROOK][D1];

//         GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];
//         GAME_STATE[CASTLING_AVAILABLE] &= 0b00000011;
//         GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];

//         _chessBoard[A1] = ES;
//         _chessBoard[E1] = ES;
//         _chessBoard[C1] = WHITE_KING;
//         _chessBoard[D1] = WHITE_ROOK;
//         GAME_STATE[SIDE] ^= 6;

//         break;

//     case 0b00000010:
//         GAME_STATE[BLACK_OCCUPANCY] ^= GAME_STATE[BLACK_KING_OCCUPANCY] | (1ULL << H8);
//         GAME_STATE[TOTAL_OCCUPANCY] ^= GAME_STATE[BLACK_KING_OCCUPANCY] | (1ULL << H8);

//         GAME_STATE[BLACK_OCCUPANCY] |= (1ULL << G8) | (1ULL << F8);

//         GAME_STATE[TOTAL_OCCUPANCY] |= (1ULL << G8) | (1ULL << F8);

//         GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= (1ULL << H8) | (1ULL << F8);
//         GAME_STATE[BLACK_KING_OCCUPANCY] = (1ULL << G8);
//         GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[BLACK_KING][E8];
//         GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[BLACK_KING][G8];
//         GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][H8];
//         GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][F8];

//         GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];
//         GAME_STATE[CASTLING_AVAILABLE] &= 0b00001100;
//         GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];

//         _chessBoard[E8] = ES;
//         _chessBoard[H8] = ES;
//         _chessBoard[G8] = BLACK_KING;
//         _chessBoard[F8] = BLACK_ROOK;
//         GAME_STATE[NUMBER_FULL_MOVES] += 1;
//         GAME_STATE[SIDE] ^= 6;

//         break;

//     case 0b00000001:
//         GAME_STATE[BLACK_OCCUPANCY] ^= GAME_STATE[BLACK_KING_OCCUPANCY] | (1ULL << A8);
//         GAME_STATE[TOTAL_OCCUPANCY] ^= GAME_STATE[BLACK_KING_OCCUPANCY] | (1ULL << A8);

//         GAME_STATE[BLACK_OCCUPANCY] |= (1ULL << C8) | (1ULL << D8);

//         GAME_STATE[TOTAL_OCCUPANCY] |= (1ULL << C8) | (1ULL << D8);

//         GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= (1ULL << A8) | (1ULL << D8);
//         GAME_STATE[BLACK_KING_OCCUPANCY] = (1ULL << C8);
//         GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[BLACK_KING][E8];
//         GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[BLACK_KING][C8];
//         GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][A8];
//         GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][D8];
//         GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];
//         GAME_STATE[CASTLING_AVAILABLE] &= 0b00001100;
//         GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];

//         _chessBoard[E8] = ES;
//         _chessBoard[A8] = ES;
//         _chessBoard[C8] = BLACK_KING;
//         _chessBoard[D8] = BLACK_ROOK;
//         GAME_STATE[NUMBER_FULL_MOVES] += 1;
//         GAME_STATE[SIDE] ^= 6;

//         break;
//     }

//     GAME_STATE[ENPASSANT_SQUARE] = NS;
// }



Square findEnpassantSquare(Square PAWN_SQUARE, Square enPassantSquare)
{


    if (enPassantSquare == NS) return NS;

    switch (GAME_STATE[SIDE])
    {
    case 0:
        if ((PAWN_SQUARE % 8) != 0 && enPassantSquare == PAWN_SQUARE + 7)
            return (Square)(PAWN_SQUARE - 1);
        if ((PAWN_SQUARE % 8) != 7 && enPassantSquare == PAWN_SQUARE + 9)
            return (Square)(PAWN_SQUARE + 1);
        break;

    case 6:
        if ((PAWN_SQUARE % 8) != 7 && enPassantSquare == PAWN_SQUARE - 7)
            return (Square)(PAWN_SQUARE + 1);
        if ((PAWN_SQUARE % 8) != 0 && enPassantSquare == PAWN_SQUARE - 9)
            return (Square)(PAWN_SQUARE - 1);
        break;
    }
    return NS;
}

// void doEnpassant( Square _pawnSrc, Square _pawnDest, Square _originDist)
// {

//     GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistSideToMove;
//     GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[_chessBoard[_pawnSrc]][_pawnSrc];

//     GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[_chessBoard[_pawnSrc]][_pawnDest];

//     GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[_chessBoard[_originDist]][_originDist];
//     GAME_STATE[NUMBER_FULL_MOVES] += GAME_STATE[SIDE] >> 2;

//     int outerSide = GAME_STATE[SIDE] ^ 6;

//     GAME_STATE[(outerSide) + 5] ^= (1ULL << _originDist);
//     GAME_STATE[((outerSide) >> 2) + WHITE_OCCUPANCY] ^= (1ULL << _originDist);
//     GAME_STATE[GAME_STATE[SIDE] + 5] ^= (1ULL << _pawnSrc) | (1ULL << _pawnDest);
//     GAME_STATE[(GAME_STATE[SIDE] >> 2) + WHITE_OCCUPANCY] ^= (1ULL << _pawnSrc) | (1ULL << _pawnDest);
//     _chessBoard[_pawnDest] = (Pieces)(GAME_STATE[SIDE] + 5);
//     _chessBoard[_pawnSrc] = ES;
//     _chessBoard[_originDist] = ES;
//     GAME_STATE[SIDE] = outerSide;

//     GAME_STATE[NUMBER_HALF_MOVES] = 0;
//     GAME_STATE[ENPASSANT_SQUARE] = NS;
//     GAME_STATE[TOTAL_OCCUPANCY] = GAME_STATE[WHITE_OCCUPANCY] | GAME_STATE[BLACK_OCCUPANCY];

// }

static inline void doEnpassant( Square _pawnSrc, Square _pawnDest, Square _originDist ,MoveList*    _moveList)
{
    uint64_t mask = 1ULL << _pawnDest;
    uint64_t _srcMask = 1ULL << _pawnSrc;
    uint64_t _originDestMask = 1ULL << _originDist;
    uint64_t _gMask = mask | _srcMask;


    int srcPiece = helperArr[_chessBoard[_pawnSrc]][0];
    int srcColor = helperArr[_chessBoard[_pawnSrc]][1];
    int destPiece=helperArr[_chessBoard[_pawnDest]][0];
    int destColor= helperArr[_chessBoard[_pawnDest]][1];
    int enDist =  helperArr[_chessBoard[_originDist]][0];
    int enColorDist =  helperArr[_chessBoard[_originDist]][1];

    GAME_STATE[srcPiece] ^= _gMask;
    GAME_STATE[srcColor] ^= _gMask;
    GAME_STATE[destPiece] ^= mask;
    GAME_STATE[destColor] ^= mask;
    GAME_STATE[enColorDist] ^= _originDestMask;
    GAME_STATE[enDist] ^= _originDestMask;

    GAME_STATE[TOTAL_OCCUPANCY] = GAME_STATE[BLACK_OCCUPANCY] | GAME_STATE[WHITE_OCCUPANCY];


    checkKingInCheckAndAddMove(GAME_STATE[SIDE] , GAME_STATE[SIDE]^6 ,_pawnSrc ,_pawnDest ,0,_moveList);


    GAME_STATE[srcPiece] ^= _gMask;
    GAME_STATE[srcColor] ^= _gMask;
    GAME_STATE[destPiece] ^= mask;
    GAME_STATE[destColor] ^= mask;
    GAME_STATE[enColorDist] ^= _originDestMask;
    GAME_STATE[enDist] ^= _originDestMask;

    GAME_STATE[TOTAL_OCCUPANCY] = GAME_STATE[BLACK_OCCUPANCY] | GAME_STATE[WHITE_OCCUPANCY];
}

static uint64_t getAttackBitBoard(int curr ,int  opp)
{
    uint64_t attacks = 0;
    uint64_t temp;
    uint64_t sameSideOccupancy = GAME_STATE[(opp >> 2) + WHITE_OCCUPANCY];
    uint64_t oppositeOccupancy = GAME_STATE[(curr >> 2) + WHITE_OCCUPANCY];

    temp = GAME_STATE[opp + 3];
    while (temp)
    {
        Square sq = __builtin_ctzll(temp);
        temp &= temp - 1;
        attacks |= getBishopAttack(sq, GAME_STATE[TOTAL_OCCUPANCY], sameSideOccupancy);
    }

    temp = GAME_STATE[opp + 2];
    while (temp)
    {
        Square sq = __builtin_ctzll(temp);
        temp &= temp - 1;
        attacks |= getRookAttack(sq, GAME_STATE[TOTAL_OCCUPANCY], sameSideOccupancy);
    }

    temp = GAME_STATE[opp + 1];
    while (temp)
    {
        Square sq = __builtin_ctzll(temp);
        temp &= temp - 1;
        attacks |= getQueenAttack(sq, GAME_STATE[TOTAL_OCCUPANCY], sameSideOccupancy);
    }

    temp = GAME_STATE[opp + 4];
    while (temp)
    {
        Square sq = __builtin_ctzll(temp);
        temp &= temp - 1;
        attacks |= getKnightAttack(sq, GAME_STATE[TOTAL_OCCUPANCY], sameSideOccupancy);
    }

    temp = GAME_STATE[opp + 5];
    while (temp)
    {
        Square sq = __builtin_ctzll(temp);
        temp &= temp - 1;
        if (opp == 0)
        {
            attacks |= whitePawnTable[sq] & ~sameSideOccupancy;
        }
        else
        {
            attacks |= blackPawnTable[sq] & ~sameSideOccupancy;
        }
    }
    temp = GAME_STATE[opp];
    Square sq = __builtin_ctzll(temp);

    attacks |= kingTable[sq] & ~sameSideOccupancy;

    return attacks;
}



static inline void  checkKingInCheckAndAddMove(int curr , int opp
                                    ,Square src , Square dest,int _promotion
                                    ,MoveList*    _moveList)
{

    if (__builtin_expect( !isTheKingInCheck(curr , opp) , 0 ))
    {
        uint16_t indiMove=0;
        indiMove |= src;
        indiMove |=(dest<<6);
        indiMove |=(_promotion<<12);

        _moveList->moves[_moveList->index++] = indiMove;
    }

}

static inline void generateBlackPieceAttack(uint64_t _temp, uint64_t _srcPieceOccupancy,
                         uint64_t _srcColorOccupancy, int _promotion,
                         uint64_t _currKingBitBoard, MoveList *_moveList,
                         int curr, int opp)
{

                Square src = __builtin_ctzll(_temp);
                _temp &= _temp - 1;
                uint64_t attacks = getBlackPawnAttack(src, GAME_STATE[TOTAL_OCCUPANCY], GAME_STATE[_srcColorOccupancy]);
                while (attacks) {
                    Square dest = __builtin_ctzll(attacks);
                    attacks &= attacks - 1;
                    uint64_t capturedColor = helperArr[_chessBoard[dest]][1];
                    uint64_t capturedPiece = helperArr[_chessBoard[dest]][0];
                    doMove(src, dest, _promotion, _srcColorOccupancy, _srcPieceOccupancy,
                           capturedColor, capturedPiece, _moveList);
                }


}

static inline void generateWhitePieceAttack(uint64_t _temp, uint64_t _srcPieceOccupancy,
                         uint64_t _srcColorOccupancy, int _promotion,
                         uint64_t _currKingBitBoard, MoveList *_moveList,
                         int curr, int opp)
{
            while (_temp) {
                Square src = (Square)__builtin_ctzll(_temp);
                _temp &= _temp - 1;
                uint64_t attacks = getWhitePawnAttack(src, GAME_STATE[TOTAL_OCCUPANCY], GAME_STATE[_srcColorOccupancy]);
                while (attacks) {
                    Square dest = (Square)__builtin_ctzll(attacks);
                    attacks &= attacks - 1;
                    uint64_t capturedColor = helperArr[_chessBoard[dest]][1];
                    uint64_t capturedPiece = helperArr[_chessBoard[dest]][0];
                    doMove(src, dest, _promotion, _srcColorOccupancy, _srcPieceOccupancy,
                           capturedColor, capturedPiece, _moveList);
                }
            }

}

// move generation

static inline void generateCastlingList(MoveList *    _moveList)
{
    if (GAME_STATE[SIDE] == 0)
    {
        uint64_t attack = getAttackBitBoard(0, 6);


        if (!(attack & GAME_STATE[WHITE_KING_OCCUPANCY]) &&
            !(attack & (1ULL << F1)) &&
            !(attack & (1ULL << G1)) &&
            (GAME_STATE[CASTLING_AVAILABLE] & 0b00001000))
        {
            if (!(GAME_STATE[TOTAL_OCCUPANCY] &
                  ((1ULL << F1) | (1ULL << G1))))
            {
                uint64_t kingMask = (1ULL << E1) | (1ULL << G1);
                uint64_t rookMask = (1ULL << H1) | (1ULL << F1);
                uint64_t occMask  = kingMask | rookMask;

                GAME_STATE[WHITE_KING_OCCUPANCY] ^= kingMask;

                GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= rookMask;

                GAME_STATE[WHITE_OCCUPANCY] ^= occMask;

                GAME_STATE[TOTAL_OCCUPANCY] =
                    GAME_STATE[WHITE_OCCUPANCY] |
                    GAME_STATE[BLACK_OCCUPANCY];

                checkKingInCheckAndAddMove(
                    0, 6, E1, G1, 0, _moveList
                );

                GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= rookMask;
                GAME_STATE[WHITE_KING_OCCUPANCY] ^= kingMask;
                GAME_STATE[WHITE_OCCUPANCY] ^= occMask;

                GAME_STATE[TOTAL_OCCUPANCY] =
                    GAME_STATE[WHITE_OCCUPANCY] |
                    GAME_STATE[BLACK_OCCUPANCY];
            }
        }


        if (!(attack & GAME_STATE[WHITE_KING_OCCUPANCY]) &&
            !(attack & (1ULL << C1)) &&
            !(attack & (1ULL << D1)) &&
            (GAME_STATE[CASTLING_AVAILABLE] & 0b00000100))
        {
            if (!(GAME_STATE[TOTAL_OCCUPANCY] &
                  ((1ULL << B1) | (1ULL << C1) | (1ULL << D1))))
            {
                uint64_t kingMask = (1ULL << E1) | (1ULL << C1);
                uint64_t rookMask = (1ULL << A1) | (1ULL << D1);
                uint64_t occMask  = kingMask | rookMask;

                GAME_STATE[WHITE_KING_OCCUPANCY] ^= kingMask;
                GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= rookMask;
                GAME_STATE[WHITE_OCCUPANCY] ^= occMask;

                GAME_STATE[TOTAL_OCCUPANCY] =
                    GAME_STATE[WHITE_OCCUPANCY] |
                    GAME_STATE[BLACK_OCCUPANCY];

                checkKingInCheckAndAddMove(
                    0, 6, E1, C1, 0, _moveList
                );

                GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= rookMask;
                GAME_STATE[WHITE_KING_OCCUPANCY] ^= kingMask;
                GAME_STATE[WHITE_OCCUPANCY] ^= occMask;

                GAME_STATE[TOTAL_OCCUPANCY] =
                    GAME_STATE[WHITE_OCCUPANCY] |
                    GAME_STATE[BLACK_OCCUPANCY];
            }
        }
    }
    else
    {
        uint64_t attack = getAttackBitBoard(6, 0);


        if (!(attack & GAME_STATE[BLACK_KING_OCCUPANCY]) &&
            !(attack & (1ULL << F8)) &&
            !(attack & (1ULL << G8)) &&
            (GAME_STATE[CASTLING_AVAILABLE] & 0b00000010))
        {
            if (!(GAME_STATE[TOTAL_OCCUPANCY] &
                  ((1ULL << F8) | (1ULL << G8))))
            {
                uint64_t kingMask = (1ULL << E8) | (1ULL << G8);
                uint64_t rookMask = (1ULL << H8) | (1ULL << F8);
                uint64_t occMask  = kingMask | rookMask;

                GAME_STATE[BLACK_KING_OCCUPANCY] ^= kingMask;
                GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= rookMask;
                GAME_STATE[BLACK_OCCUPANCY] ^= occMask;

                GAME_STATE[TOTAL_OCCUPANCY] =
                    GAME_STATE[WHITE_OCCUPANCY] |
                    GAME_STATE[BLACK_OCCUPANCY];

                checkKingInCheckAndAddMove(
                    6, 0, E8, G8, 0, _moveList
                );

                GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= rookMask;
                GAME_STATE[BLACK_KING_OCCUPANCY] ^= kingMask;
                GAME_STATE[BLACK_OCCUPANCY] ^= occMask;

                GAME_STATE[TOTAL_OCCUPANCY] =
                    GAME_STATE[WHITE_OCCUPANCY] |
                    GAME_STATE[BLACK_OCCUPANCY];
            }
        }


        if (!(attack & GAME_STATE[BLACK_KING_OCCUPANCY]) &&
            !(attack & (1ULL << C8)) &&
            !(attack & (1ULL << D8)) &&
            (GAME_STATE[CASTLING_AVAILABLE] & 0b00000001))
        {
            if (!(GAME_STATE[TOTAL_OCCUPANCY] &
                  ((1ULL << B8) | (1ULL << C8) | (1ULL << D8))))
            {
                uint64_t kingMask = (1ULL << E8) | (1ULL << C8);
                uint64_t rookMask = (1ULL << A8) | (1ULL << D8);
                uint64_t occMask  = kingMask | rookMask;

                GAME_STATE[BLACK_KING_OCCUPANCY] ^= kingMask;
                GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= rookMask;
                GAME_STATE[BLACK_OCCUPANCY] ^= occMask;

                GAME_STATE[TOTAL_OCCUPANCY] =
                    GAME_STATE[WHITE_OCCUPANCY] |
                    GAME_STATE[BLACK_OCCUPANCY];

                checkKingInCheckAndAddMove(
                    6, 0, E8, C8, 0, _moveList
                );

                GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= rookMask;
                GAME_STATE[BLACK_KING_OCCUPANCY] ^= kingMask;
                GAME_STATE[BLACK_OCCUPANCY] ^= occMask;

                GAME_STATE[TOTAL_OCCUPANCY] =
                    GAME_STATE[WHITE_OCCUPANCY] |
                    GAME_STATE[BLACK_OCCUPANCY];
            }
        }
    }
}

static inline void generateWhiteBishopMoveList(MoveList *    _moveList)
{
    __uint64_t _temp = GAME_STATE[WHITE_BISHOP];
    __uint64_t _srcPieceOccupancy = WHITE_BISHOP;
    __uint64_t totalOccupancy = GAME_STATE[TOTAL_OCCUPANCY];
    __uint64_t _srcColorOccupancy = WHITE_OCCUPANCY;

    int _promotion =0;
    while (_temp) {
        Square src = (Square)__builtin_ctzll(_temp);
        _temp &= _temp - 1;
        uint64_t attacks = getBishopAttack(src, totalOccupancy, GAME_STATE[_srcColorOccupancy]);
        while (attacks) {
            Square dest = (Square)__builtin_ctzll(attacks);
            attacks &= attacks - 1;
            uint64_t capturedColor = helperArr[_chessBoard[dest]][1];
            uint64_t capturedPiece = helperArr[_chessBoard[dest]][0];
            doMove(src, dest, _promotion, _srcColorOccupancy, _srcPieceOccupancy,
                   capturedColor, capturedPiece, _moveList);
        }
    }
    // generatePieceAttack(GAME_STATE[WHITE_BISHOP_OCCUPANCY], WHITE_BISHOP_OCCUPANCY,
    //                     WHITE_OCCUPANCY, 0, GAME_STATE[WHITE_KING_OCCUPANCY], _moveList, 0 , 6);
}

static inline void generateWhiteRookMoveList(MoveList *    _moveList)
{
    __uint64_t _temp = GAME_STATE[WHITE_ROOK];
    __uint64_t _srcPieceOccupancy = WHITE_ROOK;
    __uint64_t totalOccupancy = GAME_STATE[TOTAL_OCCUPANCY];
    __uint64_t _srcColorOccupancy = WHITE_OCCUPANCY;

    int _promotion =0;
    while (_temp) {
        Square src = (Square)__builtin_ctzll(_temp);
        _temp &= _temp - 1;
        uint64_t attacks = getRookAttack(src, totalOccupancy, GAME_STATE[_srcColorOccupancy]);
        while (attacks) {
            Square dest = (Square)__builtin_ctzll(attacks);
            attacks &= attacks - 1;
            uint64_t capturedColor = helperArr[_chessBoard[dest]][1];
            uint64_t capturedPiece = helperArr[_chessBoard[dest]][0];
            doMove(src, dest, _promotion, _srcColorOccupancy, _srcPieceOccupancy,
                   capturedColor, capturedPiece, _moveList);
        }
    }
    // generatePieceAttack(GAME_STATE[WHITE_ROOK_OCCUPANCY], WHITE_ROOK_OCCUPANCY,
    //                     WHITE_OCCUPANCY, 0, GAME_STATE[WHITE_KING_OCCUPANCY], _moveList , 0 , 6);
}

static inline void generateWhiteQueenMoveList(MoveList *    _moveList)
{
    __uint64_t _temp = GAME_STATE[WHITE_QUEEN];
    __uint64_t _srcPieceOccupancy = WHITE_QUEEN;
    __uint64_t totalOccupancy = GAME_STATE[TOTAL_OCCUPANCY];
    __uint64_t _srcColorOccupancy = WHITE_OCCUPANCY;

    int _promotion =0;
    while (_temp) {
        Square src = (Square)__builtin_ctzll(_temp);
        _temp &= _temp - 1;
        uint64_t attacks = getQueenAttack(src, totalOccupancy, GAME_STATE[_srcColorOccupancy]);
        while (attacks) {
            Square dest = (Square)__builtin_ctzll(attacks);
            attacks &= attacks - 1;
            uint64_t capturedColor = helperArr[_chessBoard[dest]][1];
            uint64_t capturedPiece = helperArr[_chessBoard[dest]][0];
            doMove(src, dest, _promotion, _srcColorOccupancy, _srcPieceOccupancy,
                   capturedColor, capturedPiece, _moveList);
        }
    }
    // generatePieceAttack(GAME_STATE[WHITE_QUEEN_OCCUPANCY], WHITE_QUEEN_OCCUPANCY,
    //                     WHITE_OCCUPANCY, 0, GAME_STATE[WHITE_KING_OCCUPANCY], _moveList , 0 , 6);
}

static inline void generateWhiteKnightMoveList(MoveList *    _moveList)
{
    __uint64_t _temp = GAME_STATE[WHITE_KNIGHT];
    __uint64_t _srcPieceOccupancy = WHITE_KNIGHT;
    __uint64_t totalOccupancy = GAME_STATE[TOTAL_OCCUPANCY];
    __uint64_t _srcColorOccupancy = WHITE_OCCUPANCY;

    int _promotion =0;
    while (_temp) {
        Square src = (Square)__builtin_ctzll(_temp);
        _temp &= _temp - 1;
        uint64_t attacks = getKnightAttack(src, totalOccupancy, GAME_STATE[_srcColorOccupancy]);
        while (attacks) {
            Square dest = (Square)__builtin_ctzll(attacks);
            attacks &= attacks - 1;
            uint64_t capturedColor = helperArr[_chessBoard[dest]][1];
            uint64_t capturedPiece = helperArr[_chessBoard[dest]][0];
            doMove(src, dest, _promotion, _srcColorOccupancy, _srcPieceOccupancy,
                   capturedColor, capturedPiece, _moveList);
        }
    }
    // generatePieceAttack(GAME_STATE[WHITE_KNIGHT_OCCUPANCY], WHITE_KNIGHT_OCCUPANCY,
    //                     WHITE_OCCUPANCY, 0, GAME_STATE[WHITE_KING_OCCUPANCY], _moveList , 0 , 6);
}

static inline void generateWhitePawnMoveList(MoveList *   _moveList)
{
    __uint64_t _temp = GAME_STATE[WHITE_PAWN];
    __uint64_t _srcPieceOccupancy = WHITE_PAWN;
    __uint64_t totalOccupancy = GAME_STATE[TOTAL_OCCUPANCY];
    __uint64_t _srcColorOccupancy = WHITE_OCCUPANCY;

    int _promotion =0;
    while (_temp) {
        Square src = (Square)__builtin_ctzll(_temp);
        _temp &= _temp - 1;
        uint64_t attacks = getWhitePawnAttack(src, totalOccupancy, GAME_STATE[_srcColorOccupancy]);
        while (attacks) {
            Square dest = (Square)__builtin_ctzll(attacks);
            attacks &= attacks - 1;
            uint64_t capturedColor = helperArr[_chessBoard[dest]][1];
            uint64_t capturedPiece = helperArr[_chessBoard[dest]][0];
            doMove(src, dest, _promotion, _srcColorOccupancy, _srcPieceOccupancy,
                   capturedColor, capturedPiece, _moveList);
        }
    }
    // generatePieceAttack(GAME_STATE[WHITE_PAWN_OCCUPANCY], WHITE_PAWN_OCCUPANCY,
    //                     WHITE_OCCUPANCY, 0, GAME_STATE[WHITE_KING_OCCUPANCY], _moveList , 0 , 6);

}


void generateKingAttack(uint64_t _temp, uint64_t _srcPieceOccupancy,
                         uint64_t _srcColorOccupancy, int _promotion,
                         uint64_t _currKingBitBoard, MoveList *   _moveList , int curr, int opp)
{
    uint64_t savedEn = GAME_STATE[ENPASSANT_SQUARE];
    while (_temp)
    {
        Square src = (Square)__builtin_ctzll(_temp);
        _temp &= _temp - 1;

        uint64_t attacks =
            getKingAttackAndMovement(src,
                                GAME_STATE[_srcColorOccupancy],
                                getAttackBitBoard(curr , opp));

        while (attacks)
        {
            Square dest = __builtin_ctzll(attacks);
            attacks &= attacks - 1;

            uint64_t capturedColor = helperArr[_chessBoard[dest]][1];
            uint64_t capturedPiece = helperArr[_chessBoard[dest]][0];

            doMove(
                src,
                dest,
                _promotion,
                _srcColorOccupancy,
                _srcPieceOccupancy,
                capturedColor,
                capturedPiece ,_moveList);
        }
    }
    GAME_STATE[ENPASSANT_SQUARE]=savedEn;
}


static inline void generateWhiteKingMoveList(MoveList *   _moveList)
{
    generateKingAttack(GAME_STATE[WHITE_KING_OCCUPANCY], WHITE_KING_OCCUPANCY,
                        WHITE_OCCUPANCY, 0, GAME_STATE[WHITE_KING_OCCUPANCY],
                        _moveList, 0 , 6);

    generateCastlingList(_moveList);
}

static inline void generateBlackKingMoveList(MoveList *   _moveList)
{
    generateKingAttack(GAME_STATE[BLACK_KING_OCCUPANCY], BLACK_KING_OCCUPANCY,
                        BLACK_OCCUPANCY, 0, GAME_STATE[BLACK_KING_OCCUPANCY],
                        _moveList, 6, 0);

    generateCastlingList(_moveList);
}

static inline void generateBlackBishopMoveList(MoveList *   _moveList)
{
    __uint64_t _temp = GAME_STATE[BLACK_BISHOP];
    __uint64_t _srcPieceOccupancy = BLACK_BISHOP;
    __uint64_t totalOccupancy = GAME_STATE[TOTAL_OCCUPANCY];
    __uint64_t _srcColorOccupancy = BLACK_OCCUPANCY;


    while (_temp) {
        Square src = __builtin_ctzll(_temp);
        _temp &= _temp - 1;
        uint64_t attacks = getBishopAttack(src, totalOccupancy, GAME_STATE[_srcColorOccupancy]);
        while (attacks) {
            Square dest =__builtin_ctzll(attacks);
            attacks &= attacks - 1;
            uint64_t capturedColor = helperArr[_chessBoard[dest]][1];
            uint64_t capturedPiece = helperArr[_chessBoard[dest]][0];
            doMove(src, dest, 0, _srcColorOccupancy, _srcPieceOccupancy,
                   capturedColor, capturedPiece, _moveList);
        }
    }

}

static inline void generateBlackRookMoveList(MoveList *   _moveList)
{
    __uint64_t _temp = GAME_STATE[BLACK_ROOK];
    __uint64_t _srcPieceOccupancy = BLACK_ROOK;
    __uint64_t totalOccupancy = GAME_STATE[TOTAL_OCCUPANCY];
    __uint64_t _srcColorOccupancy = BLACK_OCCUPANCY;

    while (_temp) {
        Square src = __builtin_ctzll(_temp);
        _temp &= _temp - 1;
        uint64_t attacks = getRookAttack(src, totalOccupancy, GAME_STATE[_srcColorOccupancy]);
        while (attacks) {
            Square dest = __builtin_ctzll(attacks);
            attacks &= attacks - 1;
            uint64_t capturedColor = helperArr[_chessBoard[dest]][1];
            uint64_t capturedPiece = helperArr[_chessBoard[dest]][0];
            doMove(src, dest, 0, _srcColorOccupancy, _srcPieceOccupancy,
                   capturedColor, capturedPiece, _moveList);
        }
    }

}

static inline void generateBlackQueenMoveList(MoveList *   _moveList)
{
    __uint64_t _temp = GAME_STATE[BLACK_QUEEN];
    __uint64_t _srcPieceOccupancy = BLACK_QUEEN;
    __uint64_t totalOccupancy = GAME_STATE[TOTAL_OCCUPANCY];
    __uint64_t _srcColorOccupancy = BLACK_OCCUPANCY;

    while (_temp) {
        Square src = __builtin_ctzll(_temp);
        _temp &= _temp - 1;
        uint64_t attacks = getQueenAttack(src, totalOccupancy, GAME_STATE[_srcColorOccupancy]);
        while (attacks) {
            Square dest = __builtin_ctzll(attacks);
            attacks &= attacks - 1;
            uint64_t capturedColor = helperArr[_chessBoard[dest]][1];
            uint64_t capturedPiece = helperArr[_chessBoard[dest]][0];
            doMove(src, dest, 0, _srcColorOccupancy, _srcPieceOccupancy,
                   capturedColor, capturedPiece, _moveList);
        }
    }
    // generatePieceAttack(GAME_STATE[BLACK_QUEEN_OCCUPANCY], BLACK_QUEEN_OCCUPANCY,
    //                     BLACK_OCCUPANCY, 0, GAME_STATE[BLACK_KING_OCCUPANCY], _moveList , 6, 0);
}

static inline void generateBlackKnightMoveList(MoveList *   _moveList)
{
    __uint64_t _temp = GAME_STATE[BLACK_KNIGHT];
    __uint64_t _srcPieceOccupancy = BLACK_KNIGHT;
    __uint64_t totalOccupancy = GAME_STATE[TOTAL_OCCUPANCY];
    __uint64_t _srcColorOccupancy = BLACK_OCCUPANCY;

    while (_temp) {
        Square src = __builtin_ctzll(_temp);
        _temp &= _temp - 1;
        uint64_t attacks = getKnightAttack(src, totalOccupancy, GAME_STATE[_srcColorOccupancy]);
        while (attacks) {
            Square dest = __builtin_ctzll(attacks);
            attacks &= attacks - 1;
            uint64_t capturedColor = helperArr[_chessBoard[dest]][1];
            uint64_t capturedPiece = helperArr[_chessBoard[dest]][0];
            doMove(src, dest, 0, _srcColorOccupancy, _srcPieceOccupancy,
                   capturedColor, capturedPiece, _moveList);
        }
    }

}

static inline void generateEnpassantMoves(MoveList *   _moveList)
{
    Square enPassant = (Square)GAME_STATE[ENPASSANT_SQUARE];
    if (enPassant == NS) return;

    int curr = GAME_STATE[SIDE] == 0 ? 0 : 6;
    int opp = GAME_STATE[SIDE] == 0 ? 6 : 0;
    uint64_t pawns = GAME_STATE[curr + 5];
    pawns &= pawnEnpassantMask[GAME_STATE[SIDE]>>2] ;

    while (pawns)
    {
        int index = __builtin_ctzll(pawns);
        pawns &= pawns - 1;

        Square epSquare = findEnpassantSquare((Square)index, enPassant);
        if (epSquare != NS)
        {
            doEnpassant((Square)index, enPassant, epSquare , _moveList);
        }
    }
}

// static inline void generatePawnMovements(MoveList *_moveList)
// {
//     generateEnpassantMoves(_moveList);

//     int side = GAME_STATE[SIDE];
//     int ownOccupancy = (side >> 2) + WHITE_OCCUPANCY;
//     int enemyOccupancy = (side == 0) ? BLACK_OCCUPANCY : WHITE_OCCUPANCY;
//     int pawnOccupancy = side + 5;

//     uint64_t pawns = GAME_STATE[pawnOccupancy];
//     uint64_t occupied = GAME_STATE[TOTAL_OCCUPANCY];
//     uint64_t own = GAME_STATE[ownOccupancy];
//     uint64_t enemy = GAME_STATE[enemyOccupancy];

//     while (pawns)
//     {
//         int index = __builtin_ctzll(pawns);
//         pawns &= pawns - 1;

//         if (side == 0)
//         {
//             int forward = index + 8;

//             if (forward < 64 && !(occupied & (1ULL << forward)))
//             {
//                 if (index >= 48)
//                 {
//                     uint64_t mask = 1ULL << forward;
//                     uint64_t srcMask = 1ULL << index;
//                     uint64_t gMask = mask | srcMask;

//                     GAME_STATE[WHITE_OCCUPANCY] ^= gMask;
//                     GAME_STATE[WHITE_PAWN_OCCUPANCY] ^= gMask;
//                     GAME_STATE[TOTAL_OCCUPANCY] =
//                         GAME_STATE[BLACK_OCCUPANCY] | GAME_STATE[WHITE_OCCUPANCY];

//                     if (__builtin_expect(!isTheKingInCheck(0, 6), 0))
//                     {
//                         uint16_t move = index | (forward << 6);

//                         for (int prom = 1; prom <= 4; prom++)
//                         {
//                             uint16_t promotedMove =
//                                 (move & 0x0FFF) | ((uint16_t)prom << 12);
//                             _moveList->moves[_moveList->index++] = promotedMove;
//                         }
//                     }

//                     GAME_STATE[WHITE_OCCUPANCY] ^= gMask;
//                     GAME_STATE[WHITE_PAWN_OCCUPANCY] ^= gMask;
//                     GAME_STATE[TOTAL_OCCUPANCY] =
//                         GAME_STATE[BLACK_OCCUPANCY] | GAME_STATE[WHITE_OCCUPANCY];
//                 }
//                 else
//                 {
//                     doMove(index, forward, 0,
//                            WHITE_OCCUPANCY, WHITE_PAWN_OCCUPANCY,
//                            NO_PIECE, NO_PIECE, _moveList);

//                     if (index >= 8 && index < 16)
//                     {
//                         int doubleForward = index + 16;

//                         if (!(occupied & (1ULL << doubleForward)))
//                         {
//                             doMove(index, doubleForward, 0,
//                                    WHITE_OCCUPANCY, WHITE_PAWN_OCCUPANCY,
//                                    NO_PIECE, NO_PIECE, _moveList);
//                         }
//                     }
//                 }
//             }

//             uint64_t attacks = getWhitePawnAttack(
//                 (Square)index,
//                 enemy,
//                 own
//             );

//             while (attacks)
//             {
//                 int dest = __builtin_ctzll(attacks);
//                 attacks &= attacks - 1;

//                 int capturedColor = helperArr[_chessBoard[dest]][0];
//                 int capturedPiece = helperArr[_chessBoard[dest]][1];

//                 if (index >= 48)
//                 {
//                     uint64_t mask = 1ULL << dest;
//                     uint64_t srcMask = 1ULL << index;
//                     uint64_t gMask = mask | srcMask;

//                     GAME_STATE[WHITE_OCCUPANCY] ^= gMask;
//                     GAME_STATE[WHITE_PAWN_OCCUPANCY] ^= gMask;
//                     GAME_STATE[capturedColor] ^= mask;
//                     GAME_STATE[capturedPiece] ^= mask;
//                     GAME_STATE[TOTAL_OCCUPANCY] =
//                         GAME_STATE[BLACK_OCCUPANCY] | GAME_STATE[WHITE_OCCUPANCY];

//                     if (__builtin_expect(!isTheKingInCheck(0, 6), 0))
//                     {
//                         uint16_t move = index | (dest << 6);

//                         for (int prom = 1; prom <= 4; prom++)
//                         {
//                             uint16_t promotedMove =
//                                 (move & 0x0FFF) | ((uint16_t)prom << 12);
//                             _moveList->moves[_moveList->index++] = promotedMove;
//                         }
//                     }

//                     GAME_STATE[WHITE_OCCUPANCY] ^= gMask;
//                     GAME_STATE[WHITE_PAWN_OCCUPANCY] ^= gMask;
//                     GAME_STATE[capturedColor] ^= mask;
//                     GAME_STATE[capturedPiece] ^= mask;
//                     GAME_STATE[TOTAL_OCCUPANCY] =
//                         GAME_STATE[BLACK_OCCUPANCY] | GAME_STATE[WHITE_OCCUPANCY];
//                 }
//                 else
//                 {
//                     doMove(index, dest, 0,
//                            WHITE_OCCUPANCY, WHITE_PAWN_OCCUPANCY,
//                            capturedColor, capturedPiece, _moveList);
//                 }
//             }
//         }
//         else
//         {
//             int forward = index - 8;

//             if (forward >= 0 && !(occupied & (1ULL << forward)))
//             {
//                 if (index < 16)
//                 {
//                     uint64_t mask = 1ULL << forward;
//                     uint64_t srcMask = 1ULL << index;
//                     uint64_t gMask = mask | srcMask;

//                     GAME_STATE[BLACK_OCCUPANCY] ^= gMask;
//                     GAME_STATE[BLACK_PAWN_OCCUPANCY] ^= gMask;
//                     GAME_STATE[TOTAL_OCCUPANCY] =
//                         GAME_STATE[BLACK_OCCUPANCY] | GAME_STATE[WHITE_OCCUPANCY];

//                     if (__builtin_expect(!isTheKingInCheck(6, 0), 0))
//                     {
//                         uint16_t move = index | (forward << 6);

//                         for (int prom = 1; prom <= 4; prom++)
//                         {
//                             uint16_t promotedMove =
//                                 (move & 0x0FFF) | ((uint16_t)prom << 12);
//                             _moveList->moves[_moveList->index++] = promotedMove;
//                         }
//                     }

//                     GAME_STATE[BLACK_OCCUPANCY] ^= gMask;
//                     GAME_STATE[BLACK_PAWN_OCCUPANCY] ^= gMask;
//                     GAME_STATE[TOTAL_OCCUPANCY] =
//                         GAME_STATE[BLACK_OCCUPANCY] | GAME_STATE[WHITE_OCCUPANCY];
//                 }
//                 else
//                 {
//                     doMove(index, forward, 0,
//                            BLACK_OCCUPANCY, BLACK_PAWN_OCCUPANCY,
//                            NO_PIECE, NO_PIECE, _moveList);

//                     if (index >= 48 && index <= 55)
//                     {
//                         int doubleForward = index - 16;

//                         if (!(occupied & (1ULL << doubleForward)))
//                         {
//                             doMove(index, doubleForward, 0,
//                                    BLACK_OCCUPANCY, BLACK_PAWN_OCCUPANCY,
//                                    NO_PIECE, NO_PIECE, _moveList);
//                         }
//                     }
//                 }
//             }

//             uint64_t attacks = getBlackPawnAttack(
//                 (Square)index,
//                 enemy,
//                 own
//             );

//             while (attacks)
//             {
//                 int dest = __builtin_ctzll(attacks);
//                 attacks &= attacks - 1;

//                 int capturedColor = helperArr[_chessBoard[dest]][0];
//                 int capturedPiece = helperArr[_chessBoard[dest]][1];

//                 if (index < 16)
//                 {
//                     uint64_t mask = 1ULL << dest;
//                     uint64_t srcMask = 1ULL << index;
//                     uint64_t gMask = mask | srcMask;

//                     GAME_STATE[BLACK_OCCUPANCY] ^= gMask;
//                     GAME_STATE[BLACK_PAWN_OCCUPANCY] ^= gMask;
//                     GAME_STATE[capturedColor] ^= mask;
//                     GAME_STATE[capturedPiece] ^= mask;
//                     GAME_STATE[TOTAL_OCCUPANCY] =
//                         GAME_STATE[BLACK_OCCUPANCY] | GAME_STATE[WHITE_OCCUPANCY];

//                     if (__builtin_expect(!isTheKingInCheck(6, 0), 0))
//                     {
//                         uint16_t move = index | (dest << 6);

//                         for (int prom = 1; prom <= 4; prom++)
//                         {
//                             uint16_t promotedMove =
//                                 (move & 0x0FFF) | ((uint16_t)prom << 12);
//                             _moveList->moves[_moveList->index++] = promotedMove;
//                         }
//                     }

//                     GAME_STATE[BLACK_OCCUPANCY] ^= gMask;
//                     GAME_STATE[BLACK_PAWN_OCCUPANCY] ^= gMask;
//                     GAME_STATE[capturedColor] ^= mask;
//                     GAME_STATE[capturedPiece] ^= mask;
//                     GAME_STATE[TOTAL_OCCUPANCY] =
//                         GAME_STATE[BLACK_OCCUPANCY] | GAME_STATE[WHITE_OCCUPANCY];
//                 }
//                 else
//                 {
//                     doMove(index, dest, 0,
//                            BLACK_OCCUPANCY, BLACK_PAWN_OCCUPANCY,
//                            capturedColor, capturedPiece, _moveList);
//                 }
//             }
//         }
//     }
// }
static inline void generatePawnMovements(MoveList *_moveList)
{
    generateEnpassantMoves(_moveList);

    const uint64_t FILE_A = 0x0101010101010101ULL;
    const uint64_t FILE_H = 0x8080808080808080ULL;
    const uint64_t RANK_1 = 0x00000000000000FFULL;
    const uint64_t RANK_2 = 0x000000000000FF00ULL;
    const uint64_t RANK_7 = 0x00FF000000000000ULL;
    const uint64_t RANK_8 = 0xFF00000000000000ULL;

    uint64_t occ = GAME_STATE[TOTAL_OCCUPANCY];
    uint64_t empty = ~occ;

    if (GAME_STATE[SIDE] == 0)
    {
        uint64_t pawns = GAME_STATE[WHITE_PAWN_OCCUPANCY];
        uint64_t enemy = GAME_STATE[BLACK_OCCUPANCY];

        uint64_t single = (pawns << 8) & empty;
        uint64_t doublePush =
            ((single & 0x0000000000FF0000ULL) << 8) & empty;

        uint64_t capturesLeft =
            (pawns << 7) & ~FILE_H & enemy;

        uint64_t capturesRight =
            (pawns << 9) & ~FILE_A & enemy;

        uint64_t moves;

        moves = single & ~RANK_8;
        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;

            doMove(dest - 8, dest, 0,
                   WHITE_OCCUPANCY,
                   WHITE_PAWN_OCCUPANCY,
                   NO_PIECE,
                   NO_PIECE,
                   _moveList);
        }

        moves = doublePush;
        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;

            doMove(dest - 16, dest, 0,
                   WHITE_OCCUPANCY,
                   WHITE_PAWN_OCCUPANCY,
                   NO_PIECE,
                   NO_PIECE,
                   _moveList);
        }

        moves = single & RANK_8;
        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;

            int src = dest - 8;

            for (int prom = 1; prom <= 4; prom++)
            {
                doMove(src, dest, prom,
                       WHITE_OCCUPANCY,
                       WHITE_PAWN_OCCUPANCY,
                       NO_PIECE,
                       NO_PIECE,
                       _moveList);
            }
        }

        moves = capturesLeft & ~RANK_8;
        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;

            int src = dest - 7;

            int capturedColor = helperArr[_chessBoard[dest]][0];
            int capturedPiece = helperArr[_chessBoard[dest]][1];

            doMove(src, dest, 0,
                   WHITE_OCCUPANCY,
                   WHITE_PAWN_OCCUPANCY,
                   capturedColor,
                   capturedPiece,
                   _moveList);
        }

        moves = capturesRight & ~RANK_8;
        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;

            int src = dest - 9;

            int capturedColor = helperArr[_chessBoard[dest]][0];
            int capturedPiece = helperArr[_chessBoard[dest]][1];

            doMove(src, dest, 0,
                   WHITE_OCCUPANCY,
                   WHITE_PAWN_OCCUPANCY,
                   capturedColor,
                   capturedPiece,
                   _moveList);
        }

        moves = capturesLeft & RANK_8;
        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;

            int src = dest - 7;

            int capturedColor = helperArr[_chessBoard[dest]][0];
            int capturedPiece = helperArr[_chessBoard[dest]][1];

            for (int prom = 1; prom <= 4; prom++)
            {
                doMove(src, dest, prom,
                       WHITE_OCCUPANCY,
                       WHITE_PAWN_OCCUPANCY,
                       capturedColor,
                       capturedPiece,
                       _moveList);
            }
        }

        moves = capturesRight & RANK_8;
        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;

            int src = dest - 9;

            int capturedColor = helperArr[_chessBoard[dest]][0];
            int capturedPiece = helperArr[_chessBoard[dest]][1];

            for (int prom = 1; prom <= 4; prom++)
            {
                doMove(src, dest, prom,
                       WHITE_OCCUPANCY,
                       WHITE_PAWN_OCCUPANCY,
                       capturedColor,
                       capturedPiece,
                       _moveList);
            }
        }
    }
    else
    {
        uint64_t pawns = GAME_STATE[BLACK_PAWN_OCCUPANCY];
        uint64_t enemy = GAME_STATE[WHITE_OCCUPANCY];

        uint64_t single = (pawns >> 8) & empty;
        uint64_t doublePush =
            ((single & 0x0000FF0000000000ULL) >> 8) & empty;

        uint64_t capturesLeft =
            (pawns >> 9) & ~FILE_H & enemy;

        uint64_t capturesRight =
            (pawns >> 7) & ~FILE_A & enemy;

        uint64_t moves;

        moves = single & ~RANK_1;
        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;

            doMove(dest + 8, dest, 0,
                   BLACK_OCCUPANCY,
                   BLACK_PAWN_OCCUPANCY,
                   NO_PIECE,
                   NO_PIECE,
                   _moveList);
        }

        moves = doublePush;
        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;

            doMove(dest + 16, dest, 0,
                   BLACK_OCCUPANCY,
                   BLACK_PAWN_OCCUPANCY,
                   NO_PIECE,
                   NO_PIECE,
                   _moveList);
        }

        moves = single & RANK_1;
        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;

            int src = dest + 8;

            for (int prom = 1; prom <= 4; prom++)
            {
                doMove(src, dest, prom,
                       BLACK_OCCUPANCY,
                       BLACK_PAWN_OCCUPANCY,
                       NO_PIECE,
                       NO_PIECE,
                       _moveList);
            }
        }

        moves = capturesLeft & ~RANK_1;
        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;

            int src = dest + 9;

            int capturedColor = helperArr[_chessBoard[dest]][0];
            int capturedPiece = helperArr[_chessBoard[dest]][1];

            doMove(src, dest, 0,
                   BLACK_OCCUPANCY,
                   BLACK_PAWN_OCCUPANCY,
                   capturedColor,
                   capturedPiece,
                   _moveList);
        }

        moves = capturesRight & ~RANK_1;
        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;

            int src = dest + 7;

            int capturedColor = helperArr[_chessBoard[dest]][0];
            int capturedPiece = helperArr[_chessBoard[dest]][1];

            doMove(src, dest, 0,
                   BLACK_OCCUPANCY,
                   BLACK_PAWN_OCCUPANCY,
                   capturedColor,
                   capturedPiece,
                   _moveList);
        }

        moves = capturesLeft & RANK_1;
        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;

            int src = dest + 9;

            int capturedColor = helperArr[_chessBoard[dest]][0];
            int capturedPiece = helperArr[_chessBoard[dest]][1];

            for (int prom = 1; prom <= 4; prom++)
            {
                doMove(src, dest, prom,
                       BLACK_OCCUPANCY,
                       BLACK_PAWN_OCCUPANCY,
                       capturedColor,
                       capturedPiece,
                       _moveList);
            }
        }

        moves = capturesRight & RANK_1;
        while (moves)
        {
            int dest = __builtin_ctzll(moves);
            moves &= moves - 1;

            int src = dest + 7;

            int capturedColor = helperArr[_chessBoard[dest]][0];
            int capturedPiece = helperArr[_chessBoard[dest]][1];

            for (int prom = 1; prom <= 4; prom++)
            {
                doMove(src, dest, prom,
                       BLACK_OCCUPANCY,
                       BLACK_PAWN_OCCUPANCY,
                       capturedColor,
                       capturedPiece,
                       _moveList);
            }
        }
    }
}

void generateMoveList(MoveList * _moves)
{

    if(__builtin_expect(isTheKingInDoubleCheck(GAME_STATE[SIDE], GAME_STATE[SIDE]^6) ,0)){
        if(GAME_STATE[SIDE]==0){
            generateWhiteKingMoveList(_moves);
            return;
        }

        generateBlackKingMoveList(_moves);
        return;
    }
    if (GAME_STATE[SIDE] == 0)
    {
        generatePawnMovements(_moves);
        generateWhiteBishopMoveList(_moves);
        generateWhiteRookMoveList(_moves);
        generateWhiteQueenMoveList(_moves);
        generateWhiteKnightMoveList(_moves);
        generateWhiteKingMoveList(_moves);
        return;
    }

        generateBlackBishopMoveList(_moves);
        generatePawnMovements(_moves);
        generateBlackRookMoveList(_moves);
        generateBlackQueenMoveList(_moves);
        generateBlackKnightMoveList(_moves);
        generateBlackKingMoveList(_moves);
}

__uint64_t makeMove(uint16_t move);

uint64_t perft(int depth)
{
    if (depth == 0)
        return 1;

    MoveList moves = {0};
    generateMoveList(&moves);

    uint64_t nodes = 0;


    for (int i = 0; i < moves.index; i++)
    {
        __uint64_t copy[30];

        memcpy(copy , GAME_STATE, 24*sizeof(__uint64_t));

        __uint64_t result =makeMove(moves.moves[i]);


        nodes += perft(depth - 1);

        memcpy(GAME_STATE,copy, 24*sizeof(__uint64_t));

        if (result >> 48)
        {
            switch (result & 0x0F)
            {
                case 1:
                    _chessBoard[E1] = WHITE_KING;
                    _chessBoard[F1] = ES;
                    _chessBoard[G1] = ES;
                    _chessBoard[H1] = WHITE_ROOK;
                    break;

                case 2:
                    _chessBoard[E1] = WHITE_KING;
                    _chessBoard[D1] = ES;
                    _chessBoard[C1] = ES;
                    _chessBoard[B1] = ES;
                    _chessBoard[A1] = WHITE_ROOK;
                    break;

                case 4:
                    _chessBoard[E8] = BLACK_KING;
                    _chessBoard[F8] = ES;
                    _chessBoard[G8] = ES;
                    _chessBoard[H8] = BLACK_ROOK;
                    break;

                case 8:
                    _chessBoard[E8] = BLACK_KING;
                    _chessBoard[D8] = ES;
                    _chessBoard[C8] = ES;
                    _chessBoard[B8] = ES;
                    _chessBoard[A8] = BLACK_ROOK;
                    break;
            }
        }else{

            Pieces srcPiece  = (result >> 40) & 63;
            Pieces destPiece = (result >> 32) & 63;
            Pieces enpassPie = (result >> 24) & 63;

            Square srcS   = (result >> 16) & 63;
            Square destS  = (result >> 8)  & 63;
            Square enpassS = result & 63;
            _chessBoard[srcS]=srcPiece;
            _chessBoard[destS]=destPiece;

            if((enpassS)){
                _chessBoard[enpassS]=enpassPie;
            }
        }

    }


    return nodes;
}


uint64_t perftBulk(int depth)
{

    if (depth == 0)
        return 1;

    MoveList moves = {0};
    generateMoveList(&moves);
    if (depth == 1)
        return moves.index;
    uint64_t nodes = 0;


    for (int i = 0; i < moves.index; i++)
    {
        __uint64_t copy[30];

        memcpy(copy , GAME_STATE, 23*sizeof(__uint64_t));

        __uint64_t result =makeMove(moves.moves[i]);


        nodes += perftBulk(depth - 1);

        memcpy(GAME_STATE,copy, 23*sizeof(__uint64_t));
        if (result >> 48)
        {
            switch (result & 0x0F)
            {
                case 1:
                    _chessBoard[E1] = WHITE_KING;
                    _chessBoard[F1] = ES;
                    _chessBoard[G1] = ES;
                    _chessBoard[H1] = WHITE_ROOK;
                    break;

                case 2:
                    _chessBoard[E1] = WHITE_KING;
                    _chessBoard[D1] = ES;
                    _chessBoard[C1] = ES;
                    _chessBoard[B1] = ES;
                    _chessBoard[A1] = WHITE_ROOK;
                    break;

                case 4:
                    _chessBoard[E8] = BLACK_KING;
                    _chessBoard[F8] = ES;
                    _chessBoard[G8] = ES;
                    _chessBoard[H8] = BLACK_ROOK;
                    break;

                case 8:
                    _chessBoard[E8] = BLACK_KING;
                    _chessBoard[D8] = ES;
                    _chessBoard[C8] = ES;
                    _chessBoard[B8] = ES;
                    _chessBoard[A8] = BLACK_ROOK;
                    break;
            }
        }
        else{

            Pieces srcPiece  = (result >> 40) & 63;
            Pieces destPiece = (result >> 32) & 63;
            Pieces enpassPie = (result >> 24) & 63;

            Square srcS   = (result >> 16) & 63;
            Square destS  = (result >> 8)  & 63;
            Square enpassS = result & 63;

            _chessBoard[srcS]=srcPiece;
            _chessBoard[destS]=destPiece;

            if((enpassS)){
                _chessBoard[enpassS]=enpassPie;
            }
        }
    }


    return nodes;
}

uint64_t divide(int depth)
{
    if (depth <= 0)
        return 1;

    MoveList moves = {0};
    generateMoveList(&moves);

    uint64_t total = 0;


    for (int i = 0; i < moves.index; i++)
    {
        __uint64_t copy[30];

        memcpy(copy , GAME_STATE, 23*sizeof(__uint64_t));

        __uint64_t result =makeMove(moves.moves[i]);


        __uint64_t nodes = perft(depth - 1);

        memcpy(GAME_STATE,copy, 23*sizeof(__uint64_t));
        if((result>>48)){
            result >>= 48;
            switch (result){
                case 1:
                    _chessBoard[E1]=WHITE_KING;
                    _chessBoard[F1]=ES;
                    _chessBoard[G1]=ES;
                    _chessBoard[H1]=WHITE_ROOK;

                    break;
                case 2:
                    _chessBoard[E1]=WHITE_KING;
                    _chessBoard[D1]=ES;
                    _chessBoard[C1]=ES;
                    _chessBoard[B1]=ES;
                    _chessBoard[A1]=WHITE_ROOK;

                    break;
                case  4:
                    _chessBoard[E8]=BLACK_KING;
                    _chessBoard[F8]=ES;
                    _chessBoard[G8]=ES;
                    _chessBoard[H8]=BLACK_ROOK;
                    break;
                case 8:
                    _chessBoard[E8]=BLACK_KING;
                    _chessBoard[D8]=ES;
                    _chessBoard[C8]=ES;
                    _chessBoard[B8]=ES;
                    _chessBoard[A8]=BLACK_ROOK;
                    break;
            }

        }else{

            Pieces srcPiece  = (result >> 40) & 63;
            Pieces destPiece = (result >> 32) & 63;
            Pieces enpassPie = (result >> 24) & 63;

            Square srcS   = (result >> 16) & 63;
            Square destS  = (result >> 8)  & 63;
            Square enpassS = result & 63;

            _chessBoard[srcS]=srcPiece;
            _chessBoard[destS]=destPiece;

            if((enpassS)){
                _chessBoard[enpassS]=enpassPie;
            }
        }


        printf("%s : %llu\n",moves.moves[i] ,
               (unsigned long long)nodes);

        total += nodes;
    }



    printf("\nTotal Nodes: %llu\n", (unsigned long long)total);

    return total;
}

uint64_t divideBulk(int depth)
{
    if (depth <= 0)
        return 1;

    MoveList moves = {0};
    generateMoveList(&moves);


    uint64_t total = 0;


    for (int i = 0; i < moves.index; i++)
    {
        __uint64_t copy[30];

        memcpy(copy , GAME_STATE, 23*sizeof(__uint64_t));

        __uint64_t result =makeMove(moves.moves[i]);


        __uint64_t nodes = perftBulk(depth - 1);

        memcpy(GAME_STATE,copy, 23*sizeof(__uint64_t));
        if((result>>48)){
            result >>= 48;
            switch (result){
                case 1:
                    _chessBoard[E1]=WHITE_KING;
                    _chessBoard[F1]=ES;
                    _chessBoard[G1]=ES;
                    _chessBoard[H1]=WHITE_ROOK;

                    break;
                case 2:
                    _chessBoard[E1]=WHITE_KING;
                    _chessBoard[D1]=ES;
                    _chessBoard[C1]=ES;
                    _chessBoard[B1]=ES;
                    _chessBoard[A1]=WHITE_ROOK;

                    break;
                case  4:
                    _chessBoard[E8]=BLACK_KING;
                    _chessBoard[F8]=ES;
                    _chessBoard[G8]=ES;
                    _chessBoard[H8]=BLACK_ROOK;
                    break;
                case 8:
                    _chessBoard[E8]=BLACK_KING;
                    _chessBoard[D8]=ES;
                    _chessBoard[C8]=ES;
                    _chessBoard[B8]=ES;
                    _chessBoard[A8]=BLACK_ROOK;
                    break;
            }

        }else{

            Pieces srcPiece  = (result >> 40) & 63;
            Pieces destPiece = (result >> 32) & 63;
            Pieces enpassPie = (result >> 24) & 63;

            Square srcS   = (result >> 16) & 63;
            Square destS  = (result >> 8)  & 63;
            Square enpassS = result & 63;

            _chessBoard[srcS]=srcPiece;
            _chessBoard[destS]=destPiece;

            if((enpassS)){
                _chessBoard[enpassS]=enpassPie;
            }
        }

        char promotionPiece[] = "\0qrbn";

        printf("%c%d%c%d%c : %llu\n",
               'a' + (moves.moves[i] & 0b00111111) % 8,
               1 + (moves.moves[i] & 0b00111111) / 8,
               'a' + ((moves.moves[i]>>6) & 0b00111111) % 8,
               1 + ((moves.moves[i]>>6) & 0b00111111) / 8,
               promotionPiece[((moves.moves[i]>>12) & 0b00111111)],
               (unsigned long long)nodes);

        total += nodes;
    }



    printf("\nTotal Nodes: %llu\n", (unsigned long long)total);

    return total;
}

// -------------------------------------------
// ------------- INITIALIZATION HELPER -------
// -------------------------------------------

void piecePuter(uint64_t PIECE_NUMBER, Pieces piece)
{
    for (int i = 0; i < 64; i++)
    {
        if (PIECE_NUMBER & (1ULL << i))
        {
            _chessBoard[i] = piece;
        }
    }
}

void emptyInitializationHelper()
{

    _globalZorbistHashing = generateZorbistNumbers();

    GAME_STATE[EVALUATION] = 1000;

    GAME_STATE[BLACK_PAWN_OCCUPANCY] = 0b0000000011111111000000000000000000000000000000000000000000000000;
    GAME_STATE[WHITE_PAWN_OCCUPANCY] = 0b0000000000000000000000000000000000000000000000001111111100000000;
    GAME_STATE[BLACK_ROOK_OCCUPANCY] = 0b1000000100000000000000000000000000000000000000000000000000000000;
    GAME_STATE[BLACK_KNIGHT_OCCUPANCY] = 0b0100001000000000000000000000000000000000000000000000000000000000;
    GAME_STATE[BLACK_BISHOP_OCCUPANCY] = 0b0010010000000000000000000000000000000000000000000000000000000000;
    GAME_STATE[BLACK_KING_OCCUPANCY] = 0b0001000000000000000000000000000000000000000000000000000000000000;
    GAME_STATE[BLACK_QUEEN_OCCUPANCY] = 0b0000100000000000000000000000000000000000000000000000000000000000;
    GAME_STATE[WHITE_ROOK_OCCUPANCY] = 0b0000000000000000000000000000000000000000000000000000000010000001;
    GAME_STATE[WHITE_KNIGHT_OCCUPANCY] = 0b0000000000000000000000000000000000000000000000000000000001000010;
    GAME_STATE[WHITE_BISHOP_OCCUPANCY] = 0b0000000000000000000000000000000000000000000000000000000000100100;
    GAME_STATE[WHITE_QUEEN_OCCUPANCY] = 0b0000000000000000000000000000000000000000000000000000000000001000;
    GAME_STATE[WHITE_KING_OCCUPANCY] = 0b0000000000000000000000000000000000000000000000000000000000010000;

    GAME_STATE[TOTAL_OCCUPANCY] =
        GAME_STATE[BLACK_PAWN_OCCUPANCY] ^ GAME_STATE[BLACK_ROOK_OCCUPANCY] ^
        GAME_STATE[BLACK_KNIGHT_OCCUPANCY] ^ GAME_STATE[BLACK_BISHOP_OCCUPANCY] ^
        GAME_STATE[BLACK_QUEEN_OCCUPANCY] ^ GAME_STATE[BLACK_KING_OCCUPANCY] ^
        GAME_STATE[WHITE_PAWN_OCCUPANCY] ^ GAME_STATE[WHITE_ROOK_OCCUPANCY] ^
        GAME_STATE[WHITE_KNIGHT_OCCUPANCY] ^ GAME_STATE[WHITE_QUEEN_OCCUPANCY] ^
        GAME_STATE[WHITE_KING_OCCUPANCY] ^ GAME_STATE[WHITE_BISHOP_OCCUPANCY];

    GAME_STATE[BLACK_OCCUPANCY] =
        GAME_STATE[BLACK_PAWN_OCCUPANCY] ^ GAME_STATE[BLACK_ROOK_OCCUPANCY] ^
        GAME_STATE[BLACK_KNIGHT_OCCUPANCY] ^ GAME_STATE[BLACK_BISHOP_OCCUPANCY] ^
        GAME_STATE[BLACK_QUEEN_OCCUPANCY] ^ GAME_STATE[BLACK_KING_OCCUPANCY];

    GAME_STATE[WHITE_OCCUPANCY] = GAME_STATE[TOTAL_OCCUPANCY] ^ GAME_STATE[BLACK_OCCUPANCY];

    GAME_STATE[CASTLING_AVAILABLE] = 0b00001111;
    GAME_STATE[SIDE] = 0;

    GAME_STATE[ZORBIST_HASH] = generateZorbistHashFromAGameState();
    GAME_STATE[ENPASSANT_SQUARE] = NS;
    GAME_STATE[NUMBER_FULL_MOVES] = 1;
    GAME_STATE[NUMBER_HALF_MOVES] = 0;
}

// ---------------------------------------------
// ------------------ EVALUATION ---------------
// ---------------------------------------------

uint64_t evaluateThisPosition()
{
    return 0;
};

void initializeHelperFunc(){
    pieceInitializer();

    piecePuter(GAME_STATE[BLACK_PAWN_OCCUPANCY], BLACK_PAWN);
    piecePuter(GAME_STATE[BLACK_ROOK_OCCUPANCY], BLACK_ROOK);
    piecePuter(GAME_STATE[BLACK_BISHOP_OCCUPANCY], BLACK_BISHOP);
    piecePuter(GAME_STATE[BLACK_KNIGHT_OCCUPANCY], BLACK_KNIGHT);
    piecePuter(GAME_STATE[BLACK_QUEEN_OCCUPANCY], BLACK_QUEEN);
    piecePuter(GAME_STATE[BLACK_KING_OCCUPANCY], BLACK_KING);
    piecePuter(GAME_STATE[WHITE_PAWN_OCCUPANCY], WHITE_PAWN);
    piecePuter(GAME_STATE[WHITE_ROOK_OCCUPANCY], WHITE_ROOK);
    piecePuter(GAME_STATE[WHITE_BISHOP_OCCUPANCY], WHITE_BISHOP);
    piecePuter(GAME_STATE[WHITE_KNIGHT_OCCUPANCY], WHITE_KNIGHT);
    piecePuter(GAME_STATE[WHITE_QUEEN_OCCUPANCY], WHITE_QUEEN);
    piecePuter(GAME_STATE[WHITE_KING_OCCUPANCY], WHITE_KING);
    piecePuter(~(GAME_STATE[TOTAL_OCCUPANCY]), ES);
}

void initializer()
{
    emptyInitializationHelper();
    initializeHelperFunc();
}


// -----------------------------------------------
// --------------- MAKE MOVE ---------------------
// -----------------------------------------------

__uint64_t makeMove(uint16_t move)
{
    char promotionPie[5] = {'\0', 'q', 'r', 'b', 'n'};
    __uint64_t result = 0;

    bool isPromotion = (move >> 12) != 0;
    char promotion = promotionPie[move >> 12];

    Square src = move & 0x3F;
    Square dest = (move >> 6) & 0x3F;

    bool isEnpassant = false;
    bool isCastle = false;

    Square oldEnpassantSquare = (Square)GAME_STATE[ENPASSANT_SQUARE];

    Pieces movingPiece = _chessBoard[src];
    Pieces capturedPiece = _chessBoard[dest];

    if ((movingPiece == WHITE_KING || movingPiece == BLACK_KING) &&
        ((src == E1 && (dest == G1 || dest == C1)) ||
         (src == E8 && (dest == G8 || dest == C8))))
    {
        isCastle = true;
    }

    GAME_STATE[ENPASSANT_SQUARE] = NS;

    if (movingPiece == WHITE_PAWN &&
        dest == oldEnpassantSquare &&
        capturedPiece == ES &&
        src >= A5 && src <= H5)
    {
        isEnpassant = true;
    }

    if (movingPiece == BLACK_PAWN &&
        dest == oldEnpassantSquare &&
        capturedPiece == ES &&
        src >= A4 && src <= H4)
    {
        isEnpassant = true;
    }

    if (movingPiece == BLACK_PAWN &&
        src >= A7 && src <= H7 &&
        dest >= A5 && dest <= H5)
    {
        GAME_STATE[ENPASSANT_SQUARE] = (Square)(src - 8);
    }
    else if (movingPiece == WHITE_PAWN &&
             src >= A2 && src <= H2 &&
             dest >= A4 && dest <= H4)
    {
        GAME_STATE[ENPASSANT_SQUARE] = (Square)(src + 8);
    }

    result |= ((__uint64_t)src) << 16;
    result |= ((__uint64_t)dest) << 8;
    result |= ((__uint64_t)(uint8_t)_chessBoard[src]) << 40;
    result |= ((__uint64_t)(uint8_t)_chessBoard[dest]) << 32;

    switch (movingPiece)
    {
        case WHITE_PAWN:
            GAME_STATE[WHITE_PAWN_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            break;

        case WHITE_KNIGHT:
            GAME_STATE[WHITE_KNIGHT_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            break;

        case WHITE_BISHOP:
            GAME_STATE[WHITE_BISHOP_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            break;

        case WHITE_ROOK:
            GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);

            if (src == A1)
                GAME_STATE[CASTLING_AVAILABLE] &= 0b1011;

            else if (src == H1)
                GAME_STATE[CASTLING_AVAILABLE] &= 0b0111;
            break;

        case WHITE_QUEEN:
            GAME_STATE[WHITE_QUEEN_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            break;

        case WHITE_KING:
            GAME_STATE[WHITE_KING_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            GAME_STATE[CASTLING_AVAILABLE] &= 0b0011;
            break;

        case BLACK_PAWN:
            GAME_STATE[BLACK_PAWN_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            break;

        case BLACK_KNIGHT:
            GAME_STATE[BLACK_KNIGHT_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            break;

        case BLACK_BISHOP:
            GAME_STATE[BLACK_BISHOP_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            break;

        case BLACK_ROOK:
            GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);

            if (src == A8)
                GAME_STATE[CASTLING_AVAILABLE] &= 0b1110;

            if (src == H8)
                GAME_STATE[CASTLING_AVAILABLE] &= 0b1101;
            break;

        case BLACK_QUEEN:
            GAME_STATE[BLACK_QUEEN_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            break;

        case BLACK_KING:
            GAME_STATE[BLACK_KING_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << src) | (1ULL << dest);
            GAME_STATE[CASTLING_AVAILABLE] &= 0b1100;
            break;

        default:
            break;
    }

    switch (capturedPiece)
    {
        case WHITE_PAWN:
            GAME_STATE[WHITE_PAWN_OCCUPANCY] ^= (1ULL << dest);
            GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << dest);
            break;

        case WHITE_KNIGHT:
            GAME_STATE[WHITE_KNIGHT_OCCUPANCY] ^= (1ULL << dest);
            GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << dest);
            break;

        case WHITE_BISHOP:
            GAME_STATE[WHITE_BISHOP_OCCUPANCY] ^= (1ULL << dest);
            GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << dest);
            break;

        case WHITE_ROOK:
            GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= (1ULL << dest);
            GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << dest);

            if (dest == A1)
                GAME_STATE[CASTLING_AVAILABLE] &= 0b1011;

            if (dest == H1)
                GAME_STATE[CASTLING_AVAILABLE] &= 0b0111;
            break;

        case WHITE_QUEEN:
            GAME_STATE[WHITE_QUEEN_OCCUPANCY] ^= (1ULL << dest);
            GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << dest);
            break;

        case BLACK_PAWN:
            GAME_STATE[BLACK_PAWN_OCCUPANCY] ^= (1ULL << dest);
            GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << dest);
            break;

        case BLACK_KNIGHT:
            GAME_STATE[BLACK_KNIGHT_OCCUPANCY] ^= (1ULL << dest);
            GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << dest);
            break;

        case BLACK_BISHOP:
            GAME_STATE[BLACK_BISHOP_OCCUPANCY] ^= (1ULL << dest);
            GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << dest);
            break;

        case BLACK_ROOK:
            GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= (1ULL << dest);
            GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << dest);

            if (dest == A8)
                GAME_STATE[CASTLING_AVAILABLE] &= 0b1110;

            if (dest == H8)
                GAME_STATE[CASTLING_AVAILABLE] &= 0b1101;
            break;

        case BLACK_QUEEN:
            GAME_STATE[BLACK_QUEEN_OCCUPANCY] ^= (1ULL << dest);
            GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << dest);
            break;

        case WHITE_KING:
        case BLACK_KING:
            break;

        default:
            break;
    }

    if (isEnpassant)
    {
        Square capturedPawnSquare;

        if (movingPiece == WHITE_PAWN)
        {
            capturedPawnSquare = (Square)(dest - 8);

            result |= (uint64_t)BLACK_PAWN << 24;

            GAME_STATE[BLACK_PAWN_OCCUPANCY] ^= (1ULL << capturedPawnSquare);
            GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << capturedPawnSquare);
        }
        else
        {
            capturedPawnSquare = (Square)(dest + 8);

            result |= (uint64_t)WHITE_PAWN << 24;

            GAME_STATE[WHITE_PAWN_OCCUPANCY] ^= (1ULL << capturedPawnSquare);
            GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << capturedPawnSquare);
        }

        result |= (uint8_t)capturedPawnSquare;
        _chessBoard[capturedPawnSquare] = ES;
    }

    _chessBoard[src] = ES;
    _chessBoard[dest] = movingPiece;

    if (isPromotion)
    {
        if (movingPiece == WHITE_PAWN)
        {
            GAME_STATE[WHITE_PAWN_OCCUPANCY] ^= (1ULL << dest);

            switch (promotion)
            {
                case 'q':
                    _chessBoard[dest] = WHITE_QUEEN;
                    GAME_STATE[WHITE_QUEEN_OCCUPANCY] |= (1ULL << dest);
                    break;

                case 'r':
                    _chessBoard[dest] = WHITE_ROOK;
                    GAME_STATE[WHITE_ROOK_OCCUPANCY] |= (1ULL << dest);
                    break;

                case 'b':
                    _chessBoard[dest] = WHITE_BISHOP;
                    GAME_STATE[WHITE_BISHOP_OCCUPANCY] |= (1ULL << dest);
                    break;

                case 'n':
                    _chessBoard[dest] = WHITE_KNIGHT;
                    GAME_STATE[WHITE_KNIGHT_OCCUPANCY] |= (1ULL << dest);
                    break;
            }
        }
        else if (movingPiece == BLACK_PAWN)
        {
            GAME_STATE[BLACK_PAWN_OCCUPANCY] ^= (1ULL << dest);

            switch (promotion)
            {
                case 'q':
                    _chessBoard[dest] = BLACK_QUEEN;
                    GAME_STATE[BLACK_QUEEN_OCCUPANCY] |= (1ULL << dest);
                    break;

                case 'r':
                    _chessBoard[dest] = BLACK_ROOK;
                    GAME_STATE[BLACK_ROOK_OCCUPANCY] |= (1ULL << dest);
                    break;

                case 'b':
                    _chessBoard[dest] = BLACK_BISHOP;
                    GAME_STATE[BLACK_BISHOP_OCCUPANCY] |= (1ULL << dest);
                    break;

                case 'n':
                    _chessBoard[dest] = BLACK_KNIGHT;
                    GAME_STATE[BLACK_KNIGHT_OCCUPANCY] |= (1ULL << dest);
                    break;
            }
        }
    }

    if (isCastle)
    {
        result = 0x0001000000000000ULL;

        if (src == E1 && dest == G1)
        {
            result |= 0x01;

            _chessBoard[H1] = ES;
            _chessBoard[F1] = WHITE_ROOK;

            GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= (1ULL << H1);
            GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= (1ULL << F1);
            GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << H1);
            GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << F1);
        }
        else if (src == E1 && dest == C1)
        {
            result |= 0x02;

            _chessBoard[A1] = ES;
            _chessBoard[D1] = WHITE_ROOK;

            GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= (1ULL << A1);
            GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= (1ULL << D1);
            GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << A1);
            GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << D1);
        }
        else if (src == E8 && dest == G8)
        {
            result |= 0x04;

            _chessBoard[H8] = ES;
            _chessBoard[F8] = BLACK_ROOK;

            GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= (1ULL << H8);
            GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= (1ULL << F8);
            GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << H8);
            GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << F8);
        }
        else if (src == E8 && dest == C8)
        {
            result |= 0x08;

            _chessBoard[A8] = ES;
            _chessBoard[D8] = BLACK_ROOK;

            GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= (1ULL << A8);
            GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= (1ULL << D8);
            GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << A8);
            GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << D8);
        }
    }

    GAME_STATE[SIDE] ^= 6;
    GAME_STATE[TOTAL_OCCUPANCY] =
        GAME_STATE[WHITE_OCCUPANCY] |
        GAME_STATE[BLACK_OCCUPANCY];

    return result;
}
