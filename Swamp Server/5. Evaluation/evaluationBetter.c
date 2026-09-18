#pragma once
#include "../1. core/evaluation.h"

int knight_adj[9] = { -200, -160, -120, -80, -40, 0, 40, 80, 120 };
int rook_adj[9] = { 150, 120, 90, 60, 30, 0, -30, -60, -90 };

static const int SafetyTable[100] = {
    0, 0, 1, 2, 3, 5, 7, 9, 12, 15,
    18, 22, 26, 30, 35, 39, 44, 50, 56, 62,
    68, 75, 82, 85, 89, 97, 105, 113, 122, 131,
    140, 150, 169, 180, 191, 202, 213, 225, 237, 248,
    260, 272, 283, 295, 307, 319, 330, 342, 354, 366,
    377, 389, 401, 412, 424, 436, 448, 459, 471, 483,
    494, 500, 500, 500, 500, 500, 500, 500, 500, 500,
    500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
    500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
    500, 500, 500, 500, 500, 500, 500, 500, 500, 500
};

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
    -30,   0,  15,  20,  20, 15,    0, -30,
    -30,   5,  10,  15,  15, 10,    5, -30,
    -40, -20,   0,   5,   5,   0,  -20, -40,
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
    -20, -10, -10,  0,   0, -10, -10, -20
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

double evaulateThisPosition()
{
    int returnValue = 0;

    returnValue += __builtin_popcountll(GAME_STATE[WHITE_KNIGHT_OCCUPANCY]) * 300;
    returnValue += __builtin_popcountll(GAME_STATE[WHITE_BISHOP_OCCUPANCY]) * 350;
    returnValue += __builtin_popcountll(GAME_STATE[WHITE_ROOK_OCCUPANCY]) * 500;
    returnValue += __builtin_popcountll(GAME_STATE[WHITE_PAWN_OCCUPANCY]) * 100;
    returnValue += __builtin_popcountll(GAME_STATE[WHITE_QUEEN_OCCUPANCY]) * 1000;

    returnValue -= __builtin_popcountll(GAME_STATE[BLACK_KNIGHT_OCCUPANCY]) * 300;
    returnValue -= __builtin_popcountll(GAME_STATE[BLACK_BISHOP_OCCUPANCY]) * 350;
    returnValue -= __builtin_popcountll(GAME_STATE[BLACK_ROOK_OCCUPANCY]) * 500;
    returnValue -= __builtin_popcountll(GAME_STATE[BLACK_PAWN_OCCUPANCY]) * 100;
    returnValue -= __builtin_popcountll(GAME_STATE[BLACK_QUEEN_OCCUPANCY]) * 1000;

    uint64_t temp = GAME_STATE[WHITE_PAWN_OCCUPANCY];

    while (temp) {
        int index = __builtin_ctzll(temp);
        temp &= temp - 1;

        returnValue += pawn_pst[index];
        returnValue += __builtin_popcountll(
            whitePawnTable[index] & ~GAME_STATE[WHITE_OCCUPANCY]
        );
    }

    temp = GAME_STATE[WHITE_KNIGHT_OCCUPANCY];

    while (temp) {
        int index = __builtin_ctzll(temp);
        temp &= temp - 1;

        returnValue += knight_pst[index];

    }

    temp = GAME_STATE[WHITE_KING_OCCUPANCY];

    while (temp) {
        int index = __builtin_ctzll(temp);
        temp &= temp - 1;

        returnValue += king_pst[index];
        returnValue += SafetyTable[index];
    }

    temp = GAME_STATE[WHITE_ROOK_OCCUPANCY];

    while (temp) {
        int index = __builtin_ctzll(temp);
        temp &= temp - 1;

        returnValue += rook_pst[index];


    }

    temp = GAME_STATE[WHITE_BISHOP_OCCUPANCY];

    while (temp) {
        int index = __builtin_ctzll(temp);
        temp &= temp - 1;

        returnValue += bishop_pst[index];


    }

    temp = GAME_STATE[WHITE_QUEEN_OCCUPANCY];

    while (temp) {
        int index = __builtin_ctzll(temp);
        temp &= temp - 1;

        returnValue += queen_pst[index];


    }

    temp = GAME_STATE[BLACK_PAWN_OCCUPANCY];

    while (temp) {
        int index = __builtin_ctzll(temp);
        temp &= temp - 1;

        returnValue -= pawn_pst[index ^ 56];
        returnValue -= __builtin_popcountll(
            blackPawnTable[index] & ~GAME_STATE[BLACK_OCCUPANCY]
        );
    }

    temp = GAME_STATE[BLACK_KNIGHT_OCCUPANCY];

    while (temp) {
        int index = __builtin_ctzll(temp);
        temp &= temp - 1;

        returnValue -= knight_pst[index ^ 56];

    }

    temp = GAME_STATE[BLACK_KING_OCCUPANCY];

    while (temp) {
        int index = __builtin_ctzll(temp);
        temp &= temp - 1;

        returnValue -= king_pst[index ^ 56];
        returnValue -= SafetyTable[index ^ 56];
    }

    temp = GAME_STATE[BLACK_ROOK_OCCUPANCY];

    while (temp) {
        int index = __builtin_ctzll(temp);
        temp &= temp - 1;

        returnValue -= rook_pst[index ^ 56];


    }

    temp = GAME_STATE[BLACK_BISHOP_OCCUPANCY];

    while (temp) {
        int index = __builtin_ctzll(temp);
        temp &= temp - 1;

        returnValue -= bishop_pst[index ^ 56];


    }

    temp = GAME_STATE[BLACK_QUEEN_OCCUPANCY];

    while (temp) {
        int index = __builtin_ctzll(temp);
        temp &= temp - 1;

        returnValue -= queen_pst[index ^ 56];




    }

    returnValue += rook_adj[
        __builtin_popcountll(GAME_STATE[WHITE_PAWN_OCCUPANCY])
    ];

    returnValue += knight_adj[
        __builtin_popcountll(GAME_STATE[WHITE_PAWN_OCCUPANCY])
    ];

    returnValue -= rook_adj[
        __builtin_popcountll(GAME_STATE[BLACK_PAWN_OCCUPANCY])
    ];

    returnValue -= knight_adj[
        __builtin_popcountll(GAME_STATE[BLACK_PAWN_OCCUPANCY])
    ];

    returnValue +=
        __builtin_popcountll(GAME_STATE[WHITE_BISHOP_OCCUPANCY]) >= 2
        ? 100
        : 0;

    returnValue -=
        __builtin_popcountll(GAME_STATE[BLACK_BISHOP_OCCUPANCY]) >= 2
        ? 100
        : 0;

    return returnValue;
}
