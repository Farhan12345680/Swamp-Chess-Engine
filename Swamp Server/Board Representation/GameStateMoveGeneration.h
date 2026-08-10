#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#ifndef uint64_t
#define uint64_t __uint64_t
#endif
#ifndef uint8_t
#define uint8_t __uint8_t
#endif

//-----------------------------------
//-----------------------------------
//---- DATA STRUCTURES & CONSTANTS---
//-----------------------------------
//-----------------------------------

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

uint8_t CASTLING_ARRAY[] = {
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

#define MAX_MOVE_COUNT 128

typedef struct
{

    uint8_t _prevCastlingState;
    uint8_t _promotion;
    uint8_t _preHalfMove;
    uint8_t _preFullMove;
    uint8_t _enpassantSquare;
    uint8_t _side;

    uint8_t _src;
    uint8_t _dest;

    uint8_t _destPiece;
    uint8_t _srcPiece;

    uint8_t _colorOccupancySRC;
    uint8_t _pieceOccupancySRC;
    uint8_t _colorOccupancyDEST;
    uint8_t _pieceOccupancyDEST;

    uint64_t _prevOccupancy;
    uint64_t _prevZorbistHash;
    uint8_t _newEnpassant; 
} Move;

typedef struct
{
    uint8_t _prevCastle;
    uint8_t _currState;
    uint8_t _enpassantSquare;
    uint64_t _prevOccupancy;
    uint8_t _side;


    int _prevFullNumber;
    int _prevHalfNumber;


} CastleMove;

typedef struct
{
    uint8_t _pawnSrc;
    uint8_t _pawnDest;
    uint8_t _originDest;
    uint8_t _side;
    uint8_t _enpassantSquare;
    uint8_t _preHalfMove;
    uint8_t _preFullMove;
    uint8_t _srcPiece;

    uint64_t _prevOccupany;
    uint64_t _prevZorbistHash;

} EnpassantMove;

// ----------------------------------------
// ------- function forward Decleration ---
//-----------------------------------------

static bool isSquareAttacked(Square square, int opp);

// ----------------------------------------
// ------------- GLOBAL VARIABLE ----------
// ----------------------------------------

uint64_t GAME_STATE[30];
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

    // first for bishop
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

    // then for rooks
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

    Square kingSq = (Square)__builtin_ctzll(GAME_STATE[curr]);

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

Move doMove(
    uint8_t _src, uint8_t _dest, uint8_t _promotion,
    uint8_t _colorOccupancySRC, uint8_t _pieceOccupancySRC,
    uint8_t _colorOccupancyDEST, uint8_t _pieceOccupancyDEST, Square EN)
{


    Move move;
    move._prevCastlingState = GAME_STATE[CASTLING_AVAILABLE];
    move._promotion = _promotion;
    move._preHalfMove = GAME_STATE[NUMBER_HALF_MOVES];
    move._preFullMove = GAME_STATE[NUMBER_FULL_MOVES];
    move._enpassantSquare = GAME_STATE[ENPASSANT_SQUARE];
    move._side = GAME_STATE[SIDE];

    move._dest = _dest;
    move._src = _src;
    move._destPiece = _chessBoard[_dest];
    move._srcPiece = _chessBoard[_src];
    move._prevZorbistHash = GAME_STATE[ZORBIST_HASH];
    move._colorOccupancySRC = _colorOccupancySRC;
    move._colorOccupancyDEST = _colorOccupancyDEST;
    move._pieceOccupancySRC = _pieceOccupancySRC;
    move._pieceOccupancyDEST = _pieceOccupancyDEST;
    move._prevOccupancy = GAME_STATE[TOTAL_OCCUPANCY];

    uint64_t mask = 1ULL << _dest;
    uint64_t _srcMask = 1ULL << _src;
    uint64_t _gMask = mask | _srcMask;

    GAME_STATE[_colorOccupancySRC] ^= _gMask;
    GAME_STATE[_pieceOccupancySRC] ^= _gMask;

    GAME_STATE[TOTAL_OCCUPANCY] ^= _srcMask;
    GAME_STATE[TOTAL_OCCUPANCY] |= mask;

    if (_colorOccupancyDEST != NO_PIECE && _pieceOccupancyDEST != NO_PIECE)
    {
        GAME_STATE[_colorOccupancyDEST] ^= mask;
        GAME_STATE[_pieceOccupancyDEST] ^= mask;
    }

    GAME_STATE[NUMBER_FULL_MOVES] += GAME_STATE[SIDE] >> 2;
    GAME_STATE[SIDE] ^= 6;

    GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];
    GAME_STATE[CASTLING_AVAILABLE] = GAME_STATE[CASTLING_AVAILABLE] & CASTLING_ARRAY[_src];
    GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];

    switch (_chessBoard[_dest])
    {
    case ES:
        GAME_STATE[NUMBER_HALF_MOVES] += 1;
        if (_chessBoard[_src] == WHITE_PAWN || _chessBoard[_src] == BLACK_PAWN)
            GAME_STATE[NUMBER_HALF_MOVES] = 0;
        break;
    default:
        GAME_STATE[NUMBER_HALF_MOVES] = 0;
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[_chessBoard[_dest]][_dest];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];
        GAME_STATE[CASTLING_AVAILABLE] = GAME_STATE[CASTLING_AVAILABLE] & CASTLING_ARRAY[_dest];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];
    }

    Pieces srcp = _chessBoard[_src];
    _chessBoard[_dest] = _chessBoard[_src];
    _chessBoard[_src] = ES;

    GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistSideToMove;
    GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[_chessBoard[_dest]][_dest];
    GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[srcp][_src]; 

    GAME_STATE[ENPASSANT_SQUARE] = EN;
    move._newEnpassant = EN;
    if (_promotion)
    {
        GAME_STATE[move._side + 5] ^= mask;
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[(Pieces)move._side + 5][_dest];
        _chessBoard[_dest] = (Pieces)(move._side + _promotion);
        GAME_STATE[(Pieces)(move._side + _promotion)] ^= mask;
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[_chessBoard[_dest]][_dest];
    }

    return move;
}

void undoMove(Move *move)
{

    GAME_STATE[CASTLING_AVAILABLE] = move->_prevCastlingState;

    uint64_t mask = 1ULL << move->_dest;
    uint64_t _srcMask = 1ULL << move->_src;
    uint64_t _gMask = mask | _srcMask;

    GAME_STATE[ENPASSANT_SQUARE] = move->_enpassantSquare;



    _chessBoard[move->_dest] = (Pieces)move->_destPiece;
    _chessBoard[move->_src] = (Pieces)move->_srcPiece;

    GAME_STATE[move->_colorOccupancySRC] ^= _gMask;
    GAME_STATE[move->_pieceOccupancySRC] ^= _gMask;

    if (move->_colorOccupancyDEST != NO_PIECE && move->_pieceOccupancyDEST != NO_PIECE)
    {
        GAME_STATE[move->_colorOccupancyDEST] ^= mask;
        GAME_STATE[move->_pieceOccupancyDEST] ^= mask;
    }

    GAME_STATE[NUMBER_FULL_MOVES] = move->_preFullMove;
    GAME_STATE[NUMBER_HALF_MOVES] = move->_preHalfMove;

    GAME_STATE[ZORBIST_HASH] = move->_prevZorbistHash;
    GAME_STATE[SIDE] = move->_side;

    int _promotion = move->_promotion;
    if (_promotion)
    {
        GAME_STATE[move->_side + 5] ^= _srcMask;
        _chessBoard[move->_src] = (Pieces)(move->_side + 5);
        GAME_STATE[move->_side + _promotion] ^= mask;
    }

    GAME_STATE[TOTAL_OCCUPANCY] = move->_prevOccupancy;
}

CastleMove doCastle(uint8_t _castleSide)
{
    CastleMove move;
    move._enpassantSquare = GAME_STATE[ENPASSANT_SQUARE];
    move._prevCastle = GAME_STATE[CASTLING_AVAILABLE];
    GAME_STATE[NUMBER_HALF_MOVES] += 1;
    move._prevOccupancy = GAME_STATE[TOTAL_OCCUPANCY];

    switch (_castleSide)
    {
    case 0b00001000:
        GAME_STATE[WHITE_OCCUPANCY] ^= GAME_STATE[WHITE_KING_OCCUPANCY] | (1ULL << H1);
        GAME_STATE[TOTAL_OCCUPANCY] ^= GAME_STATE[WHITE_KING_OCCUPANCY] | (1ULL << H1);

        GAME_STATE[WHITE_OCCUPANCY] |= (1ULL << G1) | (1ULL << F1);

        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][E1];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][G1];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[WHITE_ROOK][H1];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[WHITE_ROOK][F1];

        GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= 0b10100000;
        GAME_STATE[WHITE_KING_OCCUPANCY] = (1ULL << G1);
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];
        GAME_STATE[CASTLING_AVAILABLE] &= 0b00000011;
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];

        _chessBoard[H1] = ES;
        _chessBoard[E1] = ES;
        _chessBoard[G1] = WHITE_KING;
        _chessBoard[F1] = WHITE_ROOK;
        GAME_STATE[SIDE] ^= 6;

        break;

    case 0b00000100:
        GAME_STATE[WHITE_OCCUPANCY] ^= GAME_STATE[WHITE_KING_OCCUPANCY] | (1ULL << A1);
        GAME_STATE[TOTAL_OCCUPANCY] ^= GAME_STATE[WHITE_KING_OCCUPANCY] | (1ULL << A1);

        GAME_STATE[WHITE_OCCUPANCY] |= (1ULL << C1) | (1ULL << D1);

        GAME_STATE[TOTAL_OCCUPANCY] |= (1ULL << C1) | (1ULL << D1);

        GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= 0b00001001;
        GAME_STATE[WHITE_KING_OCCUPANCY] = (1ULL << C1);
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][E1];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][C1];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[WHITE_ROOK][A1];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[WHITE_ROOK][D1];

        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];
        GAME_STATE[CASTLING_AVAILABLE] &= 0b00000011;
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];

        _chessBoard[A1] = ES;
        _chessBoard[E1] = ES;
        _chessBoard[C1] = WHITE_KING;
        _chessBoard[D1] = WHITE_ROOK;
        GAME_STATE[SIDE] ^= 6;

        break;

    case 0b00000010:
        GAME_STATE[BLACK_OCCUPANCY] ^= GAME_STATE[BLACK_KING_OCCUPANCY] | (1ULL << H8);
        GAME_STATE[TOTAL_OCCUPANCY] ^= GAME_STATE[BLACK_KING_OCCUPANCY] | (1ULL << H8);

        GAME_STATE[BLACK_OCCUPANCY] |= (1ULL << G8) | (1ULL << F8);

        GAME_STATE[TOTAL_OCCUPANCY] |= (1ULL << G8) | (1ULL << F8);

        GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= (1ULL << H8) | (1ULL << F8);
        GAME_STATE[BLACK_KING_OCCUPANCY] = (1ULL << G8);
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[BLACK_KING][E8];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[BLACK_KING][G8];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][H8];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][F8];

        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];
        GAME_STATE[CASTLING_AVAILABLE] &= 0b00001100;
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];

        _chessBoard[E8] = ES;
        _chessBoard[H8] = ES;
        _chessBoard[G8] = BLACK_KING;
        _chessBoard[F8] = BLACK_ROOK;
        GAME_STATE[NUMBER_FULL_MOVES] += 1;
        GAME_STATE[SIDE] ^= 6;

        break;

    case 0b00000001:
        GAME_STATE[BLACK_OCCUPANCY] ^= GAME_STATE[BLACK_KING_OCCUPANCY] | (1ULL << A8);
        GAME_STATE[TOTAL_OCCUPANCY] ^= GAME_STATE[BLACK_KING_OCCUPANCY] | (1ULL << A8);

        GAME_STATE[BLACK_OCCUPANCY] |= (1ULL << C8) | (1ULL << D8);

        GAME_STATE[TOTAL_OCCUPANCY] |= (1ULL << C8) | (1ULL << D8);

        GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= (1ULL << A8) | (1ULL << D8);
        GAME_STATE[BLACK_KING_OCCUPANCY] = (1ULL << C8);
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[BLACK_KING][E8];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[BLACK_KING][C8];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][A8];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][D8];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];
        GAME_STATE[CASTLING_AVAILABLE] &= 0b00001100;
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];

        _chessBoard[E8] = ES;
        _chessBoard[A8] = ES;
        _chessBoard[C8] = BLACK_KING;
        _chessBoard[D8] = BLACK_ROOK;
        GAME_STATE[NUMBER_FULL_MOVES] += 1;
        GAME_STATE[SIDE] ^= 6;

        break;
    }

    move._currState = _castleSide;
    GAME_STATE[ENPASSANT_SQUARE] = NS;
    return move;
}

void undoCastle(CastleMove *move)
{
    GAME_STATE[NUMBER_HALF_MOVES] -= 1;
    GAME_STATE[CASTLING_AVAILABLE] = move->_prevCastle;

    switch (move->_currState)
    {
    case 0b00001000:
        GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << G1) | (1ULL << F1);

        GAME_STATE[WHITE_OCCUPANCY] |= (1ULL << H1) | (1ULL << E1);

        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][E1];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][G1];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[WHITE_ROOK][H1];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[WHITE_ROOK][F1];

        GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= 0b10100000;
        GAME_STATE[WHITE_KING_OCCUPANCY] = (1ULL << E1);
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];
        GAME_STATE[CASTLING_AVAILABLE] = move->_prevCastle;
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];
        GAME_STATE[SIDE] ^= 6;

        _chessBoard[H1] = WHITE_ROOK;
        _chessBoard[E1] = WHITE_KING;
        _chessBoard[G1] = ES;
        _chessBoard[F1] = ES;
        break;

    case 0b00000100:
        GAME_STATE[WHITE_OCCUPANCY] ^= (1ULL << C1) | (1ULL << D1);

        GAME_STATE[WHITE_OCCUPANCY] |= (1ULL << A1) | (1ULL << E1);

        GAME_STATE[WHITE_ROOK_OCCUPANCY] ^= 0b00001001;
        GAME_STATE[WHITE_KING_OCCUPANCY] = (1ULL << E1);
        GAME_STATE[SIDE] ^= 6;

        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][E1];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[WHITE_KING][C1];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[WHITE_ROOK][A1];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[WHITE_ROOK][D1];

        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];
        GAME_STATE[CASTLING_AVAILABLE] = move->_prevCastle;
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];

        _chessBoard[A1] = WHITE_ROOK;
        _chessBoard[E1] = WHITE_KING;
        _chessBoard[C1] = ES;
        _chessBoard[D1] = ES;
        break;
    case 0b00000010:
        GAME_STATE[NUMBER_FULL_MOVES] -= 1;
        GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << G8) | (1ULL << F8);
        GAME_STATE[BLACK_OCCUPANCY] |= (1ULL << E8) | (1ULL << H8);

        GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= (1ULL << H8) | (1ULL << F8);
        GAME_STATE[BLACK_KING_OCCUPANCY] = (1ULL << E8);
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[BLACK_KING][E8];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[BLACK_KING][G8];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][H8];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][F8];

        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];
        GAME_STATE[CASTLING_AVAILABLE] = move->_prevCastle;
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];
        GAME_STATE[SIDE] ^= 6;

        _chessBoard[E8] = BLACK_KING;
        _chessBoard[H8] = BLACK_ROOK;
        _chessBoard[G8] = ES;
        _chessBoard[F8] = ES;
        break;
    case 0b00000001:
        GAME_STATE[NUMBER_FULL_MOVES] -= 1;
        GAME_STATE[BLACK_OCCUPANCY] ^= (1ULL << C8) | (1ULL << D8);

        GAME_STATE[BLACK_OCCUPANCY] |= (1ULL << A8) | (1ULL << E8);
        GAME_STATE[SIDE] ^= 6;

        GAME_STATE[BLACK_ROOK_OCCUPANCY] ^= (1ULL << A8) | (1ULL << D8);
        GAME_STATE[BLACK_KING_OCCUPANCY] = (1ULL << E8);
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[BLACK_KING][E8];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[BLACK_KING][C8];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][A8];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[BLACK_ROOK][D8];
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];
        GAME_STATE[CASTLING_AVAILABLE] = move->_prevCastle;
        GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistCastlingNums[GAME_STATE[CASTLING_AVAILABLE]];

        _chessBoard[E8] = BLACK_KING;
        _chessBoard[A8] = BLACK_ROOK;
        _chessBoard[C8] = ES;
        _chessBoard[D8] = ES;
        break;

    default:
        break;
    }
    GAME_STATE[ENPASSANT_SQUARE] = move->_enpassantSquare;
    GAME_STATE[TOTAL_OCCUPANCY] = move->_prevOccupancy;
}

Square findEnpassantSquare(Square PAWN_SQUARE, Square enPassantSquare)
{
    // Verify the pawn is the correct colour
    if (_chessBoard[PAWN_SQUARE] !=
        ((GAME_STATE[SIDE] == 0) ? WHITE_PAWN : BLACK_PAWN))
        return NS;

    if (enPassantSquare == NS) return NS;

    switch (GAME_STATE[SIDE])
    {
    case 0: // White to move
        if ((PAWN_SQUARE % 8) != 0 && enPassantSquare == PAWN_SQUARE + 7)
            return (Square)(PAWN_SQUARE - 1);
        if ((PAWN_SQUARE % 8) != 7 && enPassantSquare == PAWN_SQUARE + 9)
            return (Square)(PAWN_SQUARE + 1);
        break;

    case 6: // Black to move
        if ((PAWN_SQUARE % 8) != 7 && enPassantSquare == PAWN_SQUARE - 7)
            return (Square)(PAWN_SQUARE + 1);
        if ((PAWN_SQUARE % 8) != 0 && enPassantSquare == PAWN_SQUARE - 9)
            return (Square)(PAWN_SQUARE - 1);
        break;
    }
    return NS;
}

EnpassantMove doEnpassant( Square _pawnSrc, Square _pawnDest, Square _originDist)
{   
    // printf("in do enpassant");
    // if(GAME_STATE[ENPASSANT_SQUARE]!=64){
    //     printf("enpassant found\n");
    // }


    EnpassantMove move;

    move._pawnDest = _pawnDest;
    move._pawnSrc = _pawnSrc;
    move._originDest = _originDist;
    move._enpassantSquare = GAME_STATE[ENPASSANT_SQUARE];
    move._preFullMove = GAME_STATE[NUMBER_FULL_MOVES];
    move._preHalfMove = GAME_STATE[NUMBER_HALF_MOVES];
    move._prevZorbistHash = GAME_STATE[ZORBIST_HASH];
    move._srcPiece = _chessBoard[_pawnSrc];
    move._side = GAME_STATE[SIDE];
    move._prevOccupany = GAME_STATE[TOTAL_OCCUPANCY];

    GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistSideToMove;
    GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[_chessBoard[_pawnSrc]][_pawnSrc];

    GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[_chessBoard[_pawnSrc]][_pawnDest];

    GAME_STATE[ZORBIST_HASH] ^= _globalZorbistHashing._zorbistPieces[_chessBoard[_originDist]][_originDist];
    GAME_STATE[NUMBER_FULL_MOVES] += GAME_STATE[SIDE] >> 2;

    int outerSide = GAME_STATE[SIDE] ^ 6;

    GAME_STATE[(outerSide) + 5] ^= (1ULL << _originDist);
    GAME_STATE[((outerSide) >> 2) + WHITE_OCCUPANCY] ^= (1ULL << _originDist);
    GAME_STATE[GAME_STATE[SIDE] + 5] ^= (1ULL << _pawnSrc) | (1ULL << _pawnDest);
    GAME_STATE[(GAME_STATE[SIDE] >> 2) + WHITE_OCCUPANCY] ^= (1ULL << _pawnSrc) | (1ULL << _pawnDest);
    _chessBoard[_pawnDest] = (Pieces)(GAME_STATE[SIDE] + 5);
    _chessBoard[_pawnSrc] = ES;
    _chessBoard[_originDist] = ES;
    GAME_STATE[SIDE] = outerSide;

    GAME_STATE[NUMBER_HALF_MOVES] = 0;
    GAME_STATE[ENPASSANT_SQUARE] = NS;
    GAME_STATE[TOTAL_OCCUPANCY] = GAME_STATE[WHITE_OCCUPANCY] | GAME_STATE[BLACK_OCCUPANCY];
    return move;
}

void undoEnpassant(EnpassantMove *MOVE)
{
    GAME_STATE[ZORBIST_HASH] = MOVE->_prevZorbistHash;
    GAME_STATE[NUMBER_HALF_MOVES] = MOVE->_preHalfMove;
    GAME_STATE[NUMBER_FULL_MOVES] = MOVE->_preFullMove;
    GAME_STATE[ENPASSANT_SQUARE] = MOVE->_enpassantSquare;
    GAME_STATE[SIDE] = MOVE->_side;
    GAME_STATE[TOTAL_OCCUPANCY] = MOVE->_prevOccupany;

    int outerSide = GAME_STATE[SIDE] ^ 6;

    GAME_STATE[(outerSide) + 5] ^= (1ULL << MOVE->_originDest);
    GAME_STATE[((outerSide) >> 2) + WHITE_OCCUPANCY] ^= (1ULL << MOVE->_originDest);
    GAME_STATE[GAME_STATE[SIDE] + 5] ^= (1ULL << MOVE->_pawnSrc) | (1ULL << MOVE->_pawnDest);
    GAME_STATE[(GAME_STATE[SIDE] >> 2) + WHITE_OCCUPANCY] ^= (1ULL << MOVE->_pawnSrc) | (1ULL << MOVE->_pawnDest);
    _chessBoard[MOVE->_pawnDest] = ES;
    _chessBoard[MOVE->_pawnSrc] = (Pieces)(GAME_STATE[SIDE] + 5);
    _chessBoard[MOVE->_originDest] = (Pieces)(outerSide + 5);
}


typedef struct
{
    Move _moveArray[MAX_MOVE_COUNT];
    EnpassantMove _enpassantMoveArray[MAX_MOVE_COUNT];
    CastleMove _castlingMoveArray[MAX_MOVE_COUNT];
    int index;
    int enpassantIndex;
    int castlingIndex;

} MoveList;

static uint64_t getAttackBitBoard(int curr ,int  opp)
{
    uint64_t attacks = 0;
    uint64_t temp;
    uint64_t sameSideOccupancy = GAME_STATE[(opp >> 2) + WHITE_OCCUPANCY];
    uint64_t oppositeOccupancy = GAME_STATE[(curr >> 2) + WHITE_OCCUPANCY];

    temp = GAME_STATE[opp + 3];
    while (temp)
    {
        Square sq = (Square)__builtin_ctzll(temp);
        temp &= temp - 1;
        attacks |= getBishopAttack(sq, GAME_STATE[TOTAL_OCCUPANCY], sameSideOccupancy);
    }

    temp = GAME_STATE[opp + 2];
    while (temp)
    {
        Square sq = (Square)__builtin_ctzll(temp);
        temp &= temp - 1;
        attacks |= getRookAttack(sq, GAME_STATE[TOTAL_OCCUPANCY], sameSideOccupancy);
    }

    temp = GAME_STATE[opp + 1];
    while (temp)
    {
        Square sq = (Square)__builtin_ctzll(temp);
        temp &= temp - 1;
        attacks |= getQueenAttack(sq, GAME_STATE[TOTAL_OCCUPANCY], sameSideOccupancy);
    }

    temp = GAME_STATE[opp + 4];
    while (temp)
    {
        Square sq = (Square)__builtin_ctzll(temp);
        temp &= temp - 1;
        attacks |= getKnightAttack(sq, GAME_STATE[TOTAL_OCCUPANCY], sameSideOccupancy);
    }

    temp = GAME_STATE[opp + 5];
    while (temp)
    {
        Square sq = (Square)__builtin_ctzll(temp);
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
    Square sq = (Square)__builtin_ctzll(temp);

    attacks |= kingTable[sq] & ~sameSideOccupancy;

    return attacks;
}



void generatePieceAttack(uint64_t _temp, uint64_t _srcPieceOccupancy,
                         uint64_t _srcColorOccupancy, int _promotion,
                         uint64_t _currKingBitBoard, MoveList *_moveList,
                         uint64_t (*pieceAttackFunction)(Square, uint64_t, uint64_t) , int curr, int opp)
{
    uint64_t savedEn = GAME_STATE[ENPASSANT_SQUARE]; 
    while (_temp)
    {
        Square src = (Square)__builtin_ctzll(_temp);
        _temp &= _temp - 1;

        uint64_t attacks =
            pieceAttackFunction(src,
                                GAME_STATE[TOTAL_OCCUPANCY],
                                GAME_STATE[_srcColorOccupancy]);

        while (attacks)
        {
            Square dest = (Square)__builtin_ctzll(attacks);
            attacks &= attacks - 1;

            uint64_t capturedColor = NO_PIECE;
            uint64_t capturedPiece = NO_PIECE;

            switch (_chessBoard[dest])
            {
            case ES:
                break;

            case BLACK_PAWN:
                capturedColor = BLACK_OCCUPANCY;
                capturedPiece = BLACK_PAWN_OCCUPANCY;
                break;

            case BLACK_KNIGHT:
                capturedColor = BLACK_OCCUPANCY;
                capturedPiece = BLACK_KNIGHT_OCCUPANCY;
                break;

            case BLACK_BISHOP:
                capturedColor = BLACK_OCCUPANCY;
                capturedPiece = BLACK_BISHOP_OCCUPANCY;
                break;

            case BLACK_ROOK:
                capturedColor = BLACK_OCCUPANCY;
                capturedPiece = BLACK_ROOK_OCCUPANCY;
                break;

            case BLACK_QUEEN:
                capturedColor = BLACK_OCCUPANCY;
                capturedPiece = BLACK_QUEEN_OCCUPANCY;
                break;

            case BLACK_KING:
                capturedColor = BLACK_OCCUPANCY;
                capturedPiece = BLACK_KING_OCCUPANCY;
                break;
            case WHITE_PAWN:
                capturedColor = WHITE_OCCUPANCY;
                capturedPiece = WHITE_PAWN_OCCUPANCY;
                break;

            case WHITE_KNIGHT:
                capturedColor = WHITE_OCCUPANCY;
                capturedPiece = WHITE_KNIGHT_OCCUPANCY;
                break;

            case WHITE_BISHOP:
                capturedColor = WHITE_OCCUPANCY;
                capturedPiece = WHITE_BISHOP_OCCUPANCY;
                break;

            case WHITE_ROOK:
                capturedColor = WHITE_OCCUPANCY;
                capturedPiece = WHITE_ROOK_OCCUPANCY;
                break;

            case WHITE_QUEEN:
                capturedColor = WHITE_OCCUPANCY;
                capturedPiece = WHITE_QUEEN_OCCUPANCY;
                break;

            case WHITE_KING:
                capturedColor = WHITE_OCCUPANCY;
                capturedPiece = WHITE_KING_OCCUPANCY;
                break;

            default:
                continue;
            }

            Move undo = doMove(
                src,
                dest,
                _promotion,
                _srcColorOccupancy,
                _srcPieceOccupancy,
                capturedColor,
                capturedPiece, NS);
                
            if (!isTheKingInCheck(curr , opp))
            {
                _moveList->_moveArray[_moveList->index++] = undo;
            }

            undoMove(&undo);
        }
    }
    GAME_STATE[ENPASSANT_SQUARE]=savedEn;
}

// move generation

void generateCastlingList(MoveList *_moveList)
{
    if(!(GAME_STATE[SIDE])){
        uint64_t attack = getAttackBitBoard(0 , 6);

        if ( !(attack & GAME_STATE[WHITE_KING_OCCUPANCY]) && !(attack & (1ULL << F1)) && !(attack & (1ULL << G1))  && (GAME_STATE[CASTLING_AVAILABLE] & 0b0001000))
        {
            if (!(GAME_STATE[TOTAL_OCCUPANCY] & ((1ULL << F1) | (1ULL << G1))))
            {
                CastleMove move = doCastle(0b00001000);

                if (!isTheKingInCheck(0,6))
                {
                    _moveList->_castlingMoveArray[_moveList->castlingIndex++] = move;
                }
                undoCastle(&move);
            }
        }

        if ( !(attack & GAME_STATE[WHITE_KING_OCCUPANCY]) && !(attack & (1ULL << C1)) && !(attack & (1ULL << D1))  && (GAME_STATE[CASTLING_AVAILABLE] & 0b00000100))
        {
            if (!(GAME_STATE[TOTAL_OCCUPANCY] &
                ((1ULL << B1) | (1ULL << C1) | (1ULL << D1))))
            {

                CastleMove move = doCastle(0b00000100);
                if (!isTheKingInCheck(0,6))
                {
                    _moveList->_castlingMoveArray[_moveList->castlingIndex++] = move;
                }
                undoCastle(&move);
            }
        }
    }else{
        uint64_t attack = getAttackBitBoard(6 , 0);
    if ( !(attack & GAME_STATE[BLACK_KING_OCCUPANCY]) && !(attack & (1ULL << F8)) && !(attack & (1ULL << G8))  && (GAME_STATE[CASTLING_AVAILABLE] & 0b00000010))
    {
        if (!(GAME_STATE[TOTAL_OCCUPANCY] & ((1ULL << F8) | (1ULL << G8))))
        {
            CastleMove move = doCastle(0b00000010);
            if (!isTheKingInCheck(6,0))
            {
                _moveList->_castlingMoveArray[_moveList->castlingIndex++] = move;
            }
            undoCastle(&move);
        }
    }

    if (!(attack & GAME_STATE[BLACK_KING_OCCUPANCY]) && !(attack & (1ULL << C8)) && !(attack & (1ULL << D8))  && (GAME_STATE[CASTLING_AVAILABLE] & 0b00000001))
    {
        if (!(GAME_STATE[TOTAL_OCCUPANCY] &
              ((1ULL << B8) | (1ULL << C8) | (1ULL << D8))))
        {
            CastleMove move = doCastle(0b00000001);
            if (!isTheKingInCheck(6,0))
            {
                _moveList->_castlingMoveArray[_moveList->castlingIndex++] = move;
            }
            undoCastle(&move);
        }
    }
    }



}

static inline void generateWhiteBishopMoveList(MoveList *_moveList)
{
    generatePieceAttack(GAME_STATE[WHITE_BISHOP_OCCUPANCY], WHITE_BISHOP_OCCUPANCY,
                        WHITE_OCCUPANCY, 0, GAME_STATE[WHITE_KING_OCCUPANCY], _moveList, &getBishopAttack , 0 , 6);
}

static inline void generateWhiteRookMoveList(MoveList *_moveList)
{
    generatePieceAttack(GAME_STATE[WHITE_ROOK_OCCUPANCY], WHITE_ROOK_OCCUPANCY,
                        WHITE_OCCUPANCY, 0, GAME_STATE[WHITE_KING_OCCUPANCY], _moveList, &getRookAttack , 0 , 6);
}

static inline void generateWhiteQueenMoveList(MoveList *_moveList)
{
    generatePieceAttack(GAME_STATE[WHITE_QUEEN_OCCUPANCY], WHITE_QUEEN_OCCUPANCY,
                        WHITE_OCCUPANCY, 0, GAME_STATE[WHITE_KING_OCCUPANCY], _moveList, &getQueenAttack , 0 , 6);
}

static inline void generateWhiteKnightMoveList(MoveList *_moveList)
{
    generatePieceAttack(GAME_STATE[WHITE_KNIGHT_OCCUPANCY], WHITE_KNIGHT_OCCUPANCY,
                        WHITE_OCCUPANCY, 0, GAME_STATE[WHITE_KING_OCCUPANCY], _moveList, &getKnightAttack , 0 , 6);
}

static inline void generateWhitePawnMoveList(MoveList *_moveList)
{
    generatePieceAttack(GAME_STATE[WHITE_PAWN_OCCUPANCY], WHITE_PAWN_OCCUPANCY,
                        WHITE_OCCUPANCY, 0, GAME_STATE[WHITE_KING_OCCUPANCY], _moveList, &getWhitePawnAttack , 0 , 6);
}


void generateKingAttack(uint64_t _temp, uint64_t _srcPieceOccupancy,
                         uint64_t _srcColorOccupancy, int _promotion,
                         uint64_t _currKingBitBoard, MoveList *_moveList , int curr, int opp)
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
            Square dest = (Square)__builtin_ctzll(attacks);
            attacks &= attacks - 1;

            uint64_t capturedColor = NO_PIECE;
            uint64_t capturedPiece = NO_PIECE;

            switch (_chessBoard[dest])
            {
            case ES:
                break;

            case BLACK_PAWN:
                capturedColor = BLACK_OCCUPANCY;
                capturedPiece = BLACK_PAWN_OCCUPANCY;
                break;

            case BLACK_KNIGHT:
                capturedColor = BLACK_OCCUPANCY;
                capturedPiece = BLACK_KNIGHT_OCCUPANCY;
                break;

            case BLACK_BISHOP:
                capturedColor = BLACK_OCCUPANCY;
                capturedPiece = BLACK_BISHOP_OCCUPANCY;
                break;

            case BLACK_ROOK:
                capturedColor = BLACK_OCCUPANCY;
                capturedPiece = BLACK_ROOK_OCCUPANCY;
                break;

            case BLACK_QUEEN:
                capturedColor = BLACK_OCCUPANCY;
                capturedPiece = BLACK_QUEEN_OCCUPANCY;
                break;

            case BLACK_KING:
                capturedColor = BLACK_OCCUPANCY;
                capturedPiece = BLACK_KING_OCCUPANCY;
                break;
            case WHITE_PAWN:
                capturedColor = WHITE_OCCUPANCY;
                capturedPiece = WHITE_PAWN_OCCUPANCY;
                break;

            case WHITE_KNIGHT:
                capturedColor = WHITE_OCCUPANCY;
                capturedPiece = WHITE_KNIGHT_OCCUPANCY;
                break;

            case WHITE_BISHOP:
                capturedColor = WHITE_OCCUPANCY;
                capturedPiece = WHITE_BISHOP_OCCUPANCY;
                break;

            case WHITE_ROOK:
                capturedColor = WHITE_OCCUPANCY;
                capturedPiece = WHITE_ROOK_OCCUPANCY;
                break;

            case WHITE_QUEEN:
                capturedColor = WHITE_OCCUPANCY;
                capturedPiece = WHITE_QUEEN_OCCUPANCY;
                break;

            case WHITE_KING:
                capturedColor = WHITE_OCCUPANCY;
                capturedPiece = WHITE_KING_OCCUPANCY;
                break;

            default:
                continue;
            }

            Move undo = doMove(
                src,
                dest,
                _promotion,
                _srcColorOccupancy,
                _srcPieceOccupancy,
                capturedColor,
                capturedPiece, NS);
                
            if (!isTheKingInCheck(curr , opp))
            {
                _moveList->_moveArray[_moveList->index++] = undo;
            }

            undoMove(&undo);
        }
    }
    GAME_STATE[ENPASSANT_SQUARE]=savedEn;
}


static inline void generateWhiteKingMoveList(MoveList *_moveList)
{
    generateKingAttack(GAME_STATE[WHITE_KING_OCCUPANCY], WHITE_KING_OCCUPANCY,
                        WHITE_OCCUPANCY, 0, GAME_STATE[WHITE_KING_OCCUPANCY],
                        _moveList, 0 , 6);

    generateCastlingList(_moveList);
}

static inline void generateBlackKingMoveList(MoveList *_moveList)
{
    generateKingAttack(GAME_STATE[BLACK_KING_OCCUPANCY], BLACK_KING_OCCUPANCY,
                        BLACK_OCCUPANCY, 0, GAME_STATE[BLACK_KING_OCCUPANCY],
                        _moveList, 6, 0);

    generateCastlingList(_moveList);
}

static inline void generateBlackBishopMoveList(MoveList *_moveList)
{
    generatePieceAttack(GAME_STATE[BLACK_BISHOP_OCCUPANCY], BLACK_BISHOP_OCCUPANCY,
                        BLACK_OCCUPANCY, 0, GAME_STATE[BLACK_KING_OCCUPANCY], _moveList, &getBishopAttack , 6 , 0);
}

static inline void generateBlackRookMoveList(MoveList *_moveList)
{
    generatePieceAttack(GAME_STATE[BLACK_ROOK_OCCUPANCY], BLACK_ROOK_OCCUPANCY,
                        BLACK_OCCUPANCY, 0, GAME_STATE[BLACK_KING_OCCUPANCY], _moveList, &getRookAttack , 6,0);
}

static inline void generateBlackQueenMoveList(MoveList *_moveList)
{
    generatePieceAttack(GAME_STATE[BLACK_QUEEN_OCCUPANCY], BLACK_QUEEN_OCCUPANCY,
                        BLACK_OCCUPANCY, 0, GAME_STATE[BLACK_KING_OCCUPANCY], _moveList, &getQueenAttack , 6, 0);
}

static inline void generateBlackKnightMoveList(MoveList *_moveList)
{
    generatePieceAttack(GAME_STATE[BLACK_KNIGHT_OCCUPANCY], BLACK_KNIGHT_OCCUPANCY,
                        BLACK_OCCUPANCY, 0, GAME_STATE[BLACK_KING_OCCUPANCY], _moveList, &getKnightAttack , 6, 0);
}

static inline void generateEnpassantMoves(MoveList *_moveList)
{
    Square enPassant = (Square)GAME_STATE[ENPASSANT_SQUARE];
    if (enPassant == NS) return;

    int curr = GAME_STATE[SIDE] == 0 ? 0 : 6;
    int opp = GAME_STATE[SIDE] == 0 ? 6 : 0;
    uint64_t pawns = GAME_STATE[curr + 5]; 
    while (pawns)
    {
        int index = __builtin_ctzll(pawns);
        pawns &= pawns - 1;

        Square epSquare = findEnpassantSquare((Square)index, enPassant);  // pass snapshot
        if (epSquare != NS)
        {
            EnpassantMove move = doEnpassant((Square)index, enPassant, epSquare);
            if (!isTheKingInCheck(curr, opp))
            {
                _moveList->_enpassantMoveArray[_moveList->enpassantIndex++] = move;
            }
            undoEnpassant(&move);
        }
    }
}

static inline void generatePawnMovements(MoveList *_moveList)
{
    uint64_t savedEq = GAME_STATE[ENPASSANT_SQUARE];
        generateEnpassantMoves(_moveList);

    if (GAME_STATE[SIDE] == 0) 
    {
        uint64_t pawns = GAME_STATE[WHITE_PAWN_OCCUPANCY];

        while (pawns)
        {
            int index = __builtin_ctzll(pawns);
            pawns &= (pawns - 1);


            // int epSquare = findEnpassantSquare((Square)index);
            // if (epSquare != NS)
            // {
            //     EnpassantMove move = doEnpassant((Square)index, (Square)GAME_STATE[ENPASSANT_SQUARE], (Square)epSquare);

            //     undoEnpassant(&move);
            // }


            if (_chessBoard[index + 8] == ES && index < 48)
            {
                Move move = doMove(index, index + 8, 0,
                                   WHITE_OCCUPANCY, WHITE_PAWN_OCCUPANCY,
                                   NO_PIECE, NO_PIECE, NS);
                if (!isTheKingInCheck(0, 6))
                    _moveList->_moveArray[_moveList->index++] = move;
                undoMove(&move);
            }


            if (index > 7 && index < 16 &&
                _chessBoard[index + 8] == ES && _chessBoard[index + 16] == ES)
            {
                Move move = doMove(index, index + 16, 0,
                                   WHITE_OCCUPANCY, WHITE_PAWN_OCCUPANCY,
                                   NO_PIECE, NO_PIECE, index + 8);

                if (!isTheKingInCheck(0, 6))
                {
                    _moveList->_moveArray[_moveList->index++] = move;


                }
                undoMove(&move);
            }


            if (_chessBoard[index + 8] == ES && index >= 48)
            {
                for (int prom = 1; prom <= 4; prom++)
                {
                    Move move = doMove(index, index + 8, prom,
                                       WHITE_OCCUPANCY, WHITE_PAWN_OCCUPANCY,
                                       NO_PIECE, NO_PIECE, NS);
                    if (!isTheKingInCheck(0, 6))
                        _moveList->_moveArray[_moveList->index++] = move;
                    undoMove(&move);
                }
            }


            if (index + 8 < 56)   
            {
                generatePieceAttack(1ULL << index, WHITE_PAWN_OCCUPANCY,
                                    WHITE_OCCUPANCY, 0,
                                    GAME_STATE[WHITE_KING_OCCUPANCY],
                                    _moveList, &getWhitePawnAttack, 0, 6);
            }
            else  
            {
                for (int prom = 1; prom <= 4; prom++)
                {
                    generatePieceAttack(1ULL << index, WHITE_PAWN_OCCUPANCY,
                                        WHITE_OCCUPANCY, prom,
                                        GAME_STATE[WHITE_KING_OCCUPANCY],
                                        _moveList, &getWhitePawnAttack, 0, 6);
                }
            }
        }
    }
    else  
    {
        uint64_t pawns = GAME_STATE[BLACK_PAWN_OCCUPANCY];

        while (pawns)
        {
            int index = __builtin_ctzll(pawns);
            pawns &= (pawns - 1);



            if (index >= 8 && _chessBoard[index - 8] == ES)
            {
                Move move = doMove(index, index - 8, 0,
                                   BLACK_OCCUPANCY, BLACK_PAWN_OCCUPANCY,
                                   NO_PIECE, NO_PIECE, NS);


                if (!isTheKingInCheck(6, 0))
                    _moveList->_moveArray[_moveList->index++] = move;
                undoMove(&move);
            }


            if (index >= 48 && index <= 55 &&
                _chessBoard[index - 8] == ES && _chessBoard[index - 16] == ES)
            {
                Move move = doMove(index, index - 16, 0,
                                   BLACK_OCCUPANCY, BLACK_PAWN_OCCUPANCY,
                                   NO_PIECE, NO_PIECE, index - 8);
                if (!isTheKingInCheck(6, 0))
                    {
                        _moveList->_moveArray[_moveList->index++] = move;
                    }
                undoMove(&move);
            }


            if (index < 16 && _chessBoard[index - 8] == ES)
            {
                for (int prom = 1; prom <= 4; prom++)
                {
                    Move move = doMove(index, index - 8, prom,
                                       BLACK_OCCUPANCY, BLACK_PAWN_OCCUPANCY,
                                       NO_PIECE, NO_PIECE, NS);
                    if (!isTheKingInCheck(6, 0))
                        _moveList->_moveArray[_moveList->index++] = move;
                    undoMove(&move);
                }
            }


            if (index - 8 >= 8)   
            {
                generatePieceAttack(1ULL << index, BLACK_PAWN_OCCUPANCY,
                                    BLACK_OCCUPANCY, 0,
                                    GAME_STATE[BLACK_KING_OCCUPANCY],
                                    _moveList, &getBlackPawnAttack, 6, 0);
            }
            else  
            {
                for (int prom = 1; prom <= 4; prom++)
                {
                    generatePieceAttack(1ULL << index, BLACK_PAWN_OCCUPANCY,
                                        BLACK_OCCUPANCY, prom,
                                        GAME_STATE[BLACK_KING_OCCUPANCY],
                                        _moveList, &getBlackPawnAttack, 6, 0);
                }
            }
        }
    }
    GAME_STATE[ENPASSANT_SQUARE]=savedEq;
}

void generateMoveList(MoveList *_moves)
{
        uint64_t savedEn = GAME_STATE[ENPASSANT_SQUARE];   

    if (GAME_STATE[SIDE] == 0)
    {
        generatePawnMovements(_moves);
        generateWhiteBishopMoveList(_moves);
        generateWhiteRookMoveList(_moves);
        generateWhiteQueenMoveList(_moves);
        generateWhiteKnightMoveList(_moves);
        generateWhiteKingMoveList(_moves);
    }
    else
    {
        generateBlackBishopMoveList(_moves);
        generatePawnMovements(_moves);
        generateBlackRookMoveList(_moves);
        generateBlackQueenMoveList(_moves);
        generateBlackKnightMoveList(_moves);
        generateBlackKingMoveList(_moves);
    }
    GAME_STATE[ENPASSANT_SQUARE] = savedEn;

}

uint64_t perft(int depth)
{           

    if (depth == 0)
        return 1;

    MoveList moves = {0};
    generateMoveList(&moves);

    uint64_t nodes = 0;

    // Normal moves
    for (int i = 0; i < moves.index; i++)
    {
        Move *move = &moves._moveArray[i];

        doMove(move->_src,
               move->_dest,
               move->_promotion,
               move->_colorOccupancySRC,
               move->_pieceOccupancySRC,
               move->_colorOccupancyDEST,
               move->_pieceOccupancyDEST, move->_newEnpassant);

        nodes += perft(depth - 1);

        undoMove(move);

    }

    for (int i = 0; i < moves.enpassantIndex; i++)
    {
        EnpassantMove *move = &moves._enpassantMoveArray[i];

        
        doEnpassant((Square)move->_pawnSrc,
                    (Square)move->_pawnDest,
                    (Square)move->_originDest);

        nodes += perft(depth - 1);

        undoEnpassant(move);

    }

    // Castling
    for (int i = 0; i < moves.castlingIndex; i++)
    {
        CastleMove *move = &moves._castlingMoveArray[i];

        doCastle(move->_currState);

        nodes += perft(depth - 1);

        undoCastle(move);

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
        Move *move = &moves._moveArray[i];

        doMove(move->_src,
               move->_dest,
               move->_promotion,
               move->_colorOccupancySRC,
               move->_pieceOccupancySRC,
               move->_colorOccupancyDEST,
               move->_pieceOccupancyDEST, move->_newEnpassant);

        uint64_t nodes = perft(depth - 1);

        undoMove(move);

        printf("%c%d%c%d : %llu\n",
               'a' + move->_src % 8,
               1 + move->_src / 8,
               'a' + move->_dest % 8,
               1 + move->_dest / 8,
               (unsigned long long)nodes);

        total += nodes;
    }

    // En passant
    for (int i = 0; i < moves.enpassantIndex; i++)
    {
        EnpassantMove *move = &moves._enpassantMoveArray[i];

        doEnpassant(move->_pawnSrc,
                    move->_pawnDest,
                    move->_originDest);

        uint64_t nodes = perft(depth - 1);

        undoEnpassant(move);

        printf("%c%d%c%d ep : %llu\n",
               'a' + move->_pawnSrc % 8,
               1 + move->_pawnSrc / 8,
               'a' + move->_pawnDest % 8,
               1 + move->_pawnDest / 8,
               (unsigned long long)nodes);

        total += nodes;
    }

    for (int i = 0; i < moves.castlingIndex; i++)
    {
        CastleMove *move = &moves._castlingMoveArray[i];

        doCastle(move->_currState);

        uint64_t nodes = perft(depth - 1);

        undoCastle(move);

        switch (move->_currState)
        {
        case 0b00001000:
            printf("e1g1 : %llu\n", (unsigned long long)nodes);
            break;
        case 0b00000100:
            printf("e1c1 : %llu\n", (unsigned long long)nodes);
            break;
        case 0b00000010:
            printf("e8g8 : %llu\n", (unsigned long long)nodes);
            break;
        case 0b00000001:
            printf("e8c8 : %llu\n", (unsigned long long)nodes);
            break;
        }

        total += nodes;
    }

    printf("\nTotal Nodes: %llu\n", (unsigned long long)total);

    return total;
}

uint64_t perftBULK(int depth)
{           

    if (depth == 1){
        MoveList moves = {0};
        generateMoveList(&moves);
        return moves.index+moves.enpassantIndex+moves.castlingIndex;

    }

    MoveList moves = {0};
    generateMoveList(&moves);

    uint64_t nodes = 0;

    // Normal moves
    for (int i = 0; i < moves.index; i++)
    {
        Move *move = &moves._moveArray[i];

        doMove(move->_src,
               move->_dest,
               move->_promotion,
               move->_colorOccupancySRC,
               move->_pieceOccupancySRC,
               move->_colorOccupancyDEST,
               move->_pieceOccupancyDEST, move->_newEnpassant);

        nodes += perftBULK(depth - 1);

        undoMove(move);

    }

    for (int i = 0; i < moves.enpassantIndex; i++)
    {
        EnpassantMove *move = &moves._enpassantMoveArray[i];

        
        doEnpassant((Square)move->_pawnSrc,
                    (Square)move->_pawnDest,
                    (Square)move->_originDest);

        nodes += perftBULK(depth - 1);

        undoEnpassant(move);

    }

    // Castling
    for (int i = 0; i < moves.castlingIndex; i++)
    {
        CastleMove *move = &moves._castlingMoveArray[i];

        doCastle(move->_currState);

        nodes += perftBULK(depth - 1);

        undoCastle(move);

    }

    return nodes;
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
        piecePuter(~(0ULL), ES);

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

