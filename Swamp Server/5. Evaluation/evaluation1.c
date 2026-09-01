#pragma once
#include "../1. core/evaluation.h"



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

double evaulateThisPosition()
{
    double returnValue=0;

    returnValue+= __builtin_popcountll(GAME_STATE[WHITE_KING_OCCUPANCY]) * 10 ;
    returnValue+= __builtin_popcountll(GAME_STATE[WHITE_KNIGHT_OCCUPANCY]) * 300;
    returnValue+= __builtin_popcountll(GAME_STATE[WHITE_BISHOP_OCCUPANCY]) *  350;
    returnValue+= __builtin_popcountll(GAME_STATE[WHITE_ROOK_OCCUPANCY]) * 500 ;
    returnValue+=__builtin_popcountll(GAME_STATE[WHITE_PAWN_OCCUPANCY]) * 100;
    returnValue+=__builtin_popcountll(GAME_STATE[WHITE_QUEEN_OCCUPANCY]) * 1000;

    returnValue+= __builtin_popcountll(GAME_STATE[BLACK_KING_OCCUPANCY]) * -10 ;
    returnValue+= __builtin_popcountll(GAME_STATE[BLACK_KNIGHT_OCCUPANCY]) * -300;
    returnValue+= __builtin_popcountll(GAME_STATE[BLACK_BISHOP_OCCUPANCY]) *  -350;
    returnValue+= __builtin_popcountll(GAME_STATE[BLACK_ROOK_OCCUPANCY]) * -500 ;
    returnValue+= __builtin_popcountll(GAME_STATE[BLACK_PAWN_OCCUPANCY]) * -100;
    returnValue+=__builtin_popcountll(GAME_STATE[BLACK_QUEEN_OCCUPANCY]) * -1000 ;

    uint64_t temp = GAME_STATE[WHITE_PAWN_OCCUPANCY];
    while(temp>0){
        int index = __builtin_ctzll(temp);
        temp &= (temp-1);
        returnValue += pawn_pst[index];
    }

    temp = GAME_STATE[WHITE_KNIGHT_OCCUPANCY];
    while(temp>0){
        int index = __builtin_ctzll(temp);
        temp &= (temp-1);
        returnValue += knight_pst[index];
    }

    temp = GAME_STATE[WHITE_KING_OCCUPANCY];
    while(temp>0){
        int index = __builtin_ctzll(temp);
        temp &= (temp-1);
        returnValue += king_pst[index];
    }


    temp = GAME_STATE[WHITE_ROOK_OCCUPANCY];
    while(temp>0){
        int index = __builtin_ctzll(temp);
        temp &= (temp-1);
        returnValue += rook_pst[index];
    }


    temp = GAME_STATE[WHITE_BISHOP_OCCUPANCY];
    while(temp>0){
        int index = __builtin_ctzll(temp);
        temp &= (temp-1);
        returnValue += bishop_pst[index];
    }


    temp = GAME_STATE[WHITE_QUEEN_OCCUPANCY];
    while(temp>0){
        int index = __builtin_ctzll(temp);
        temp &= (temp-1);
        returnValue += queen_pst[index];
    }

    temp = GAME_STATE[BLACK_PAWN_OCCUPANCY];
    while(temp > 0){
        int index = __builtin_ctzll(temp);
        temp &= (temp-1);
        returnValue -= pawn_pst[index ^ 56];
    }

    temp = GAME_STATE[BLACK_KNIGHT_OCCUPANCY];
    while(temp>0){
        int index = __builtin_ctzll(temp);
        temp &= (temp-1);
        returnValue -= knight_pst[index ^ 56];
    }

    temp = GAME_STATE[BLACK_KING_OCCUPANCY];
    while(temp>0){
        int index = __builtin_ctzll(temp);
        temp &= (temp-1);
        returnValue -= king_pst[index ^ 56];
    }


    temp = GAME_STATE[BLACK_ROOK_OCCUPANCY];
    while(temp>0){
        int index = __builtin_ctzll(temp);
        temp &= (temp-1);
        returnValue -= rook_pst[index ^ 56];
    }


    temp = GAME_STATE[BLACK_BISHOP_OCCUPANCY];
    while(temp> 0){
        int index = __builtin_ctzll(temp);
        temp &= (temp-1);
        returnValue -= bishop_pst[index ^ 56];
    }


    temp = GAME_STATE[BLACK_QUEEN_OCCUPANCY];
    while(temp> 0){
        int index = __builtin_ctzll(temp);
        temp &= (temp-1);
        returnValue -= queen_pst[index ^ 56];
    }

    return returnValue;
}
