#pragma once
#include "../1. core/evaluation.h"

#include <stdint.h>


static const int pawn_pst[64] = {
      0,   0,   0,   0,   0,   0,   0,   0,
     50,  50,  50,  50,  50,  50,  50,  50,
     10,  10,  20,  30,  30,  20,  10,  10,
      5,   5,  10,  25,  25,  10,   5,   5,
      0,   0,   0,  20,  20,   0,   0,   0,
      5,  -5, -10,   0,   0, -10,  -5,   5,
      5,  10,  10, -20, -20,  10,  10,   5,
      0,   0,   0,   0,   0,   0,   0,   0
};

static const int knight_pst[64] = {
    -50, -40, -30, -30, -30, -30, -40, -50,
    -40, -20,   0,   0,   0,   0, -20, -40,
    -30,   0,  10,  15,  15,  10,   0, -30,
    -30,   5,  15,  20,  20,  15,   5, -30,
    -30,   0,  15,  20,  20,  15,   0, -30,
    -30,   5,  10,  15,  15,  10,   5, -30,
    -40, -20,   0,   5,   5,   0, -20, -40,
    -50, -40, -30, -30, -30, -30, -40, -50
};

static const int bishop_pst[64] = {
    -20, -10, -10, -10, -10, -10, -10, -20,
    -10,   5,   0,   0,   0,   0,   5, -10,
    -10,  10,  10,  10,  10,  10,  10, -10,
    -10,   0,  10,  10,  10,  10,   0, -10,
    -10,   5,   5,  10,  10,   5,   5, -10,
    -10,   0,   5,  10,  10,   5,   0, -10,
    -10,   0,   0,   0,   0,   0,   0, -10,
    -20, -10, -10, -10, -10, -10, -10, -20
};

static const int rook_pst[64] = {
     0,   0,   0,   5,   5,   0,   0,   0,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
     5,  10,  10,  10,  10,  10,  10,   5,
     0,   0,   0,   0,   0,   0,   0,   0
};

static const int queen_pst[64] = {
    -20, -10, -10,  0,   0, -10, -10, -20,
    -10,   0,   5,   0,   0,   0,   0, -10,
    -10,   5,   5,   5,   5,   5,   0, -10,
      0,   0,   5,   5,   5,   5,   0,  -5,
     -5,   0,   5,   5,   5,   5,   0,  -5,
    -10,   0,   5,   5,   5,   5,   0, -10,
    -10,   0,   0,   0,   0,   0,   0, -10,
    -20, -10, -10,   0,   0, -10, -10, -20
};

static const int king_pst[64] = {
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -20, -30, -30, -40, -40, -30, -30, -20,
    -10, -20, -20, -20, -20, -20, -20, -10,
     20,  20,   0,   0,   0,   0,  20,  20,
     20,  30,  10,   0,   0,  10,  30,  20
};



static const int king_end_pst[64] = {
    -50, -30, -20, -20, -20, -20, -30, -50,
    -30, -10,   0,   5,   5,   0, -10, -30,
    -20,   0,  10,  15,  15,  10,   0, -20,
    -20,   5,  15,  20,  20,  15,   5, -20,
    -20,   5,  15,  20,  20,  15,   5, -20,
    -20,   0,  10,  15,  15,  10,   0, -20,
    -30, -10,   0,   5,   5,   0, -10, -30,
    -50, -30, -20, -20, -20, -20, -30, -50
};




#define PAWN_VALUE    100
#define KNIGHT_VALUE  320
#define BISHOP_VALUE  330
#define ROOK_VALUE    500
#define QUEEN_VALUE   900
#define KING_VALUE      0




#define BISHOP_PAIR_BONUS          35
#define KNIGHT_OUTPOST_BONUS       18

#define ISOLATED_PAWN_PENALTY      12
#define DOUBLED_PAWN_PENALTY       10

#define PASSED_PAWN_BONUS_BASE     15
#define PASSED_PAWN_PER_RANK        8

#define ROOK_OPEN_FILE_BONUS       22
#define ROOK_SEMIOPEN_FILE_BONUS   12

#define KNIGHT_MOBILITY_BONUS       4
#define BISHOP_MOBILITY_BONUS       4
#define ROOK_MOBILITY_BONUS         2
#define QUEEN_MOBILITY_BONUS        1

#define KING_ACTIVITY_BONUS         2




static inline int fileOf(int sq)
{
    return sq & 7;
}

static inline int rankOf(int sq)
{
    return sq >> 3;
}



static inline int kingSquare(uint64_t bb)
{
    return __builtin_ctzll(bb);
}



static inline int countBits(uint64_t bb)
{
    return __builtin_popcountll(bb);
}



static inline int isBlackPawnAttackingSquare(uint64_t blackPawns, int sq)
{
    int file = fileOf(sq);
    int rank = rankOf(sq);


    int attackerRank = rank - 1;


    attackerRank = rank - 1;

    if (attackerRank < 0)
        return 0;

    if (file > 0) {
        int s = attackerRank * 8 + (file - 1);

        if (blackPawns & (1ULL << s))
            return 1;
    }

    if (file < 7) {
        int s = attackerRank * 8 + (file + 1);

        if (blackPawns & (1ULL << s))
            return 1;
    }

    return 0;
}


static inline int isWhitePawnAttackingSquare(uint64_t whitePawns, int sq)
{
    int file = fileOf(sq);
    int rank = rankOf(sq);


    int attackerRank = rank + 1;

    if (attackerRank > 7)
        return 0;

    if (file > 0) {
        int s = attackerRank * 8 + (file - 1);

        if (whitePawns & (1ULL << s))
            return 1;
    }

    if (file < 7) {
        int s = attackerRank * 8 + (file + 1);

        if (whitePawns & (1ULL << s))
            return 1;
    }

    return 0;
}


static inline int isIsolatedPawn(uint64_t friendlyPawns, int sq)
{
    int file = fileOf(sq);

    uint64_t leftFile  = 0;
    uint64_t rightFile = 0;

    if (file > 0) {
        leftFile = 0x0101010101010101ULL << (file - 1);
    }

    if (file < 7) {
        rightFile = 0x0101010101010101ULL << (file + 1);
    }

    return (friendlyPawns & (leftFile | rightFile)) == 0;
}



static inline int isWhitePassedPawn(uint64_t blackPawns, int sq)
{
    int file = fileOf(sq);
    int rank = rankOf(sq);

    for (int r = 0; r < rank; r++) {

        if (file > 0) {
            int s = r * 8 + file - 1;

            if (blackPawns & (1ULL << s))
                return 0;
        }

        {
            int s = r * 8 + file;

            if (blackPawns & (1ULL << s))
                return 0;
        }

        if (file < 7) {
            int s = r * 8 + file + 1;

            if (blackPawns & (1ULL << s))
                return 0;
        }
    }

    return 1;
}


static inline int isBlackPassedPawn(uint64_t whitePawns, int sq)
{
    int file = fileOf(sq);
    int rank = rankOf(sq);

    for (int r = rank + 1; r < 8; r++) {

        if (file > 0) {
            int s = r * 8 + file - 1;

            if (whitePawns & (1ULL << s))
                return 0;
        }

        {
            int s = r * 8 + file;

            if (whitePawns & (1ULL << s))
                return 0;
        }

        if (file < 7) {
            int s = r * 8 + file + 1;

            if (whitePawns & (1ULL << s))
                return 0;
        }
    }

    return 1;
}



static inline uint64_t slidingAttacks(
    int sq,
    uint64_t occupancy,
    int bishop)
{
    static const int bishopDirs[4][2] = {
        {  1,  1 },
        {  1, -1 },
        { -1,  1 },
        { -1, -1 }
    };

    static const int rookDirs[4][2] = {
        {  1,  0 },
        { -1,  0 },
        {  0,  1 },
        {  0, -1 }
    };

    const int (*dirs)[2] = bishop ? bishopDirs : rookDirs;

    uint64_t attacks = 0;

    int startFile = fileOf(sq);
    int startRank = rankOf(sq);

    for (int d = 0; d < 4; d++) {

        int file = startFile;
        int rank = startRank;

        while (1) {

            file += dirs[d][0];
            rank += dirs[d][1];

            if (file < 0 || file > 7 ||
                rank < 0 || rank > 7)
                break;

            int target = rank * 8 + file;

            attacks |= 1ULL << target;

            if (occupancy & (1ULL << target))
                break;
        }
    }

    return attacks;
}



static inline uint64_t allWhitePieces(void)
{
    return
        GAME_STATE[WHITE_KING_OCCUPANCY]   |
        GAME_STATE[WHITE_QUEEN_OCCUPANCY]  |
        GAME_STATE[WHITE_ROOK_OCCUPANCY]   |
        GAME_STATE[WHITE_BISHOP_OCCUPANCY] |
        GAME_STATE[WHITE_KNIGHT_OCCUPANCY] |
        GAME_STATE[WHITE_PAWN_OCCUPANCY];
}


static inline uint64_t allBlackPieces(void)
{
    return
        GAME_STATE[BLACK_KING_OCCUPANCY]   |
        GAME_STATE[BLACK_QUEEN_OCCUPANCY]  |
        GAME_STATE[BLACK_ROOK_OCCUPANCY]   |
        GAME_STATE[BLACK_BISHOP_OCCUPANCY] |
        GAME_STATE[BLACK_KNIGHT_OCCUPANCY] |
        GAME_STATE[BLACK_PAWN_OCCUPANCY];
}




static int evaluateMobility(
    uint64_t whitePieces,
    uint64_t blackPieces,
    uint64_t allPieces)
{
    int score = 0;



    uint64_t bb = GAME_STATE[WHITE_KNIGHT_OCCUPANCY];

    while (bb) {

        int sq = __builtin_ctzll(bb);

        bb &= bb - 1;

        int file = fileOf(sq);
        int rank = rankOf(sq);

        uint64_t attacks = 0;

        static const int knightDirs[8][2] = {
            {  1,  2 },
            {  2,  1 },
            {  2, -1 },
            {  1, -2 },
            { -1, -2 },
            { -2, -1 },
            { -2,  1 },
            { -1,  2 }
        };

        for (int i = 0; i < 8; i++) {

            int f = file + knightDirs[i][0];
            int r = rank + knightDirs[i][1];

            if (f >= 0 && f < 8 &&
                r >= 0 && r < 8) {

                attacks |= 1ULL << (r * 8 + f);
            }
        }

        attacks &= ~whitePieces;

        score += countBits(attacks) * KNIGHT_MOBILITY_BONUS;
    }


    bb = GAME_STATE[BLACK_KNIGHT_OCCUPANCY];

    while (bb) {

        int sq = __builtin_ctzll(bb);

        bb &= bb - 1;

        int file = fileOf(sq);
        int rank = rankOf(sq);

        uint64_t attacks = 0;

        static const int knightDirs[8][2] = {
            {  1,  2 },
            {  2,  1 },
            {  2, -1 },
            {  1, -2 },
            { -1, -2 },
            { -2, -1 },
            { -2,  1 },
            { -1,  2 }
        };

        for (int i = 0; i < 8; i++) {

            int f = file + knightDirs[i][0];
            int r = rank + knightDirs[i][1];

            if (f >= 0 && f < 8 &&
                r >= 0 && r < 8) {

                attacks |= 1ULL << (r * 8 + f);
            }
        }

        attacks &= ~blackPieces;

        score -= countBits(attacks) * KNIGHT_MOBILITY_BONUS;
    }



    bb = GAME_STATE[WHITE_BISHOP_OCCUPANCY];

    while (bb) {

        int sq = __builtin_ctzll(bb);

        bb &= bb - 1;

        uint64_t attacks =
            slidingAttacks(sq, allPieces, 1);

        attacks &= ~whitePieces;

        score += countBits(attacks) * BISHOP_MOBILITY_BONUS;
    }


    bb = GAME_STATE[BLACK_BISHOP_OCCUPANCY];

    while (bb) {

        int sq = __builtin_ctzll(bb);

        bb &= bb - 1;

        uint64_t attacks =
            slidingAttacks(sq, allPieces, 1);

        attacks &= ~blackPieces;

        score -= countBits(attacks) * BISHOP_MOBILITY_BONUS;
    }



    bb = GAME_STATE[WHITE_ROOK_OCCUPANCY];

    while (bb) {

        int sq = __builtin_ctzll(bb);

        bb &= bb - 1;

        uint64_t attacks =
            slidingAttacks(sq, allPieces, 0);

        attacks &= ~whitePieces;

        score += countBits(attacks) * ROOK_MOBILITY_BONUS;
    }


    bb = GAME_STATE[BLACK_ROOK_OCCUPANCY];

    while (bb) {

        int sq = __builtin_ctzll(bb);

        bb &= bb - 1;

        uint64_t attacks =
            slidingAttacks(sq, allPieces, 0);

        attacks &= ~blackPieces;

        score -= countBits(attacks) * ROOK_MOBILITY_BONUS;
    }



    bb = GAME_STATE[WHITE_QUEEN_OCCUPANCY];

    while (bb) {

        int sq = __builtin_ctzll(bb);

        bb &= bb - 1;

        uint64_t bishop =
            slidingAttacks(sq, allPieces, 1);

        uint64_t rook =
            slidingAttacks(sq, allPieces, 0);

        uint64_t attacks =
            (bishop | rook) & ~whitePieces;

        score += countBits(attacks) * QUEEN_MOBILITY_BONUS;
    }


    bb = GAME_STATE[BLACK_QUEEN_OCCUPANCY];

    while (bb) {

        int sq = __builtin_ctzll(bb);

        bb &= bb - 1;

        uint64_t bishop =
            slidingAttacks(sq, allPieces, 1);

        uint64_t rook =
            slidingAttacks(sq, allPieces, 0);

        uint64_t attacks =
            (bishop | rook) & ~blackPieces;

        score -= countBits(attacks) * QUEEN_MOBILITY_BONUS;
    }


    return score;
}



double evaulateThisPosition()
{
    double returnValue = 0.0;


    uint64_t whitePawns =
        GAME_STATE[WHITE_PAWN_OCCUPANCY];

    uint64_t blackPawns =
        GAME_STATE[BLACK_PAWN_OCCUPANCY];

    uint64_t whiteKnights =
        GAME_STATE[WHITE_KNIGHT_OCCUPANCY];

    uint64_t blackKnights =
        GAME_STATE[BLACK_KNIGHT_OCCUPANCY];

    uint64_t whiteBishops =
        GAME_STATE[WHITE_BISHOP_OCCUPANCY];

    uint64_t blackBishops =
        GAME_STATE[BLACK_BISHOP_OCCUPANCY];

    uint64_t whiteRooks =
        GAME_STATE[WHITE_ROOK_OCCUPANCY];

    uint64_t blackRooks =
        GAME_STATE[BLACK_ROOK_OCCUPANCY];

    uint64_t whiteQueens =
        GAME_STATE[WHITE_QUEEN_OCCUPANCY];

    uint64_t blackQueens =
        GAME_STATE[BLACK_QUEEN_OCCUPANCY];

    uint64_t whiteKing =
        GAME_STATE[WHITE_KING_OCCUPANCY];

    uint64_t blackKing =
        GAME_STATE[BLACK_KING_OCCUPANCY];


    uint64_t whitePieces =
        allWhitePieces();

    uint64_t blackPieces =
        allBlackPieces();

    uint64_t allPieces =
        whitePieces | blackPieces;




    returnValue += countBits(whitePawns)   * PAWN_VALUE;
    returnValue += countBits(whiteKnights) * KNIGHT_VALUE;
    returnValue += countBits(whiteBishops) * BISHOP_VALUE;
    returnValue += countBits(whiteRooks)   * ROOK_VALUE;
    returnValue += countBits(whiteQueens)  * QUEEN_VALUE;

    returnValue -= countBits(blackPawns)   * PAWN_VALUE;
    returnValue -= countBits(blackKnights) * KNIGHT_VALUE;
    returnValue -= countBits(blackBishops) * BISHOP_VALUE;
    returnValue -= countBits(blackRooks)   * ROOK_VALUE;
    returnValue -= countBits(blackQueens)  * QUEEN_VALUE;




    uint64_t temp;



    temp = whitePawns;

    while (temp) {

        int sq = __builtin_ctzll(temp);

        temp &= temp - 1;

        returnValue += pawn_pst[sq];
    }



    temp = blackPawns;

    while (temp) {

        int sq = __builtin_ctzll(temp);

        temp &= temp - 1;

        returnValue -= pawn_pst[sq ^ 56];
    }


    temp = whiteKnights;

    while (temp) {

        int sq = __builtin_ctzll(temp);

        temp &= temp - 1;

        returnValue += knight_pst[sq];
    }



    temp = blackKnights;

    while (temp) {

        int sq = __builtin_ctzll(temp);

        temp &= temp - 1;

        returnValue -= knight_pst[sq ^ 56];
    }



    temp = whiteBishops;

    while (temp) {

        int sq = __builtin_ctzll(temp);

        temp &= temp - 1;

        returnValue += bishop_pst[sq];
    }



    temp = blackBishops;

    while (temp) {

        int sq = __builtin_ctzll(temp);

        temp &= temp - 1;

        returnValue -= bishop_pst[sq ^ 56];
    }


    temp = whiteRooks;

    while (temp) {

        int sq = __builtin_ctzll(temp);

        temp &= temp - 1;

        returnValue += rook_pst[sq];
    }



    temp = blackRooks;

    while (temp) {

        int sq = __builtin_ctzll(temp);

        temp &= temp - 1;

        returnValue -= rook_pst[sq ^ 56];
    }



    temp = whiteQueens;

    while (temp) {

        int sq = __builtin_ctzll(temp);

        temp &= temp - 1;

        returnValue += queen_pst[sq];
    }



    temp = blackQueens;

    while (temp) {

        int sq = __builtin_ctzll(temp);

        temp &= temp - 1;

        returnValue -= queen_pst[sq ^ 56];
    }



    if (countBits(whiteBishops) >= 2)
        returnValue += BISHOP_PAIR_BONUS;

    if (countBits(blackBishops) >= 2)
        returnValue -= BISHOP_PAIR_BONUS;



    temp = whitePawns;

    while (temp) {

        int sq = __builtin_ctzll(temp);

        temp &= temp - 1;


        if (isIsolatedPawn(whitePawns, sq))
            returnValue -= ISOLATED_PAWN_PENALTY;



        if (isWhitePassedPawn(blackPawns, sq)) {

            int rank = rankOf(sq);


            int advancement = 6 - rank;

            if (advancement < 0)
                advancement = 0;

            returnValue +=
                PASSED_PAWN_BONUS_BASE +
                advancement * PASSED_PAWN_PER_RANK;
        }
    }


    temp = blackPawns;

    while (temp) {

        int sq = __builtin_ctzll(temp);

        temp &= temp - 1;

        if (isIsolatedPawn(blackPawns, sq))
            returnValue += ISOLATED_PAWN_PENALTY;


        if (isBlackPassedPawn(whitePawns, sq)) {

            int rank = rankOf(sq);

            int advancement = rank - 1;

            if (advancement < 0)
                advancement = 0;

            returnValue -=
                PASSED_PAWN_BONUS_BASE +
                advancement * PASSED_PAWN_PER_RANK;
        }
    }



    for (int file = 0; file < 8; file++) {

        uint64_t fileMask =
            0x0101010101010101ULL << file;

        int whiteCount =
            countBits(whitePawns & fileMask);

        int blackCount =
            countBits(blackPawns & fileMask);


        if (whiteCount > 1)
            returnValue -=
                (whiteCount - 1) * DOUBLED_PAWN_PENALTY;

        if (blackCount > 1)
            returnValue +=
                (blackCount - 1) * DOUBLED_PAWN_PENALTY;
    }



    temp = whiteRooks;

    while (temp) {

        int sq = __builtin_ctzll(temp);

        temp &= temp - 1;

        int file = fileOf(sq);

        uint64_t fileMask =
            0x0101010101010101ULL << file;

        int ownPawns =
            countBits(whitePawns & fileMask);

        int enemyPawns =
            countBits(blackPawns & fileMask);

        if (ownPawns == 0) {

            if (enemyPawns == 0)
                returnValue += ROOK_OPEN_FILE_BONUS;
            else
                returnValue += ROOK_SEMIOPEN_FILE_BONUS;
        }
    }


    temp = blackRooks;

    while (temp) {

        int sq = __builtin_ctzll(temp);

        temp &= temp - 1;

        int file = fileOf(sq);

        uint64_t fileMask =
            0x0101010101010101ULL << file;

        int ownPawns =
            countBits(blackPawns & fileMask);

        int enemyPawns =
            countBits(whitePawns & fileMask);

        if (ownPawns == 0) {

            if (enemyPawns == 0)
                returnValue -= ROOK_OPEN_FILE_BONUS;
            else
                returnValue -= ROOK_SEMIOPEN_FILE_BONUS;
        }
    }



    temp = whiteKnights;

    while (temp) {

        int sq = __builtin_ctzll(temp);

        temp &= temp - 1;

        int rank = rankOf(sq);


        if (rank >= 2 && rank <= 4) {

            if (!isBlackPawnAttackingSquare(blackPawns, sq) &&
                isWhitePawnAttackingSquare(whitePawns, sq)) {

                returnValue += KNIGHT_OUTPOST_BONUS;
            }
        }
    }


    temp = blackKnights;

    while (temp) {

        int sq = __builtin_ctzll(temp);

        temp &= temp - 1;

        int rank = rankOf(sq);

        if (rank >= 3 && rank <= 5) {

            if (!isWhitePawnAttackingSquare(whitePawns, sq) &&
                isBlackPawnAttackingSquare(blackPawns, sq)) {

                returnValue -= KNIGHT_OUTPOST_BONUS;
            }
        }
    }


    returnValue += evaluateMobility(
        whitePieces,
        blackPieces,
        allPieces
    );



    int phase = 0;

    phase += countBits(whiteQueens)  * 4;
    phase += countBits(blackQueens)  * 4;

    phase += countBits(whiteRooks)   * 2;
    phase += countBits(blackRooks)   * 2;

    phase += countBits(whiteBishops);
    phase += countBits(blackBishops);

    phase += countBits(whiteKnights);
    phase += countBits(blackKnights);


    if (phase > 24)
        phase = 24;


    if (whiteKing) {

        int sq = kingSquare(whiteKing);

        int mg = king_pst[sq];
        int eg = king_end_pst[sq];

        int tapered =
            (mg * phase +
             eg * (24 - phase)) / 24;

        returnValue += tapered;
    }


    if (blackKing) {

        int sq = kingSquare(blackKing);

        int mg = king_pst[sq ^ 56];
        int eg = king_end_pst[sq ^ 56];

        int tapered =
            (mg * phase +
             eg * (24 - phase)) / 24;

        returnValue -= tapered;
    }

    if (phase <= 8) {

        int wk = kingSquare(whiteKing);
        int bk = kingSquare(blackKing);

        int wf = fileOf(wk);
        int wr = rankOf(wk);

        int bf = fileOf(bk);
        int br = rankOf(bk);

        int whiteCentralization =
            3 - abs(wf - 3) +
            3 - abs(wr - 3);

        int blackCentralization =
            3 - abs(bf - 3) +
            3 - abs(br - 3);

        returnValue +=
            (whiteCentralization -
             blackCentralization) *
            KING_ACTIVITY_BONUS;
    }


    return returnValue;
}
