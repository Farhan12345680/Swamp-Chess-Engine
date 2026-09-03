#include "../1. core/search.h"

#include <stdint.h>
#include <string.h>


uint16_t goSearch()
{
    double evaluationValue = -100000;
    uint16_t returnValue=0xFFFF;
    MoveList moves;
    memset(&moves, 0, sizeof(MoveList));

    generateMoveList(&moves);

    for (int i=0 ;i< moves.index; i++ ){


        uint64_t tempGameState[24];
        uint8_t  tempBoard[64];
        memcpy(tempGameState, GAME_STATE , sizeof(uint64_t)*24);
        memcpy(tempBoard, _chessBoard, sizeof(uint8_t)*64);
        makeMove(moves.moves[i]);
        double temp = evaulateThisPosition();

        if( (evaluationValue < ((GAME_STATE[SIDE]==6 ? 1 :-1 )  * temp))){
            evaluationValue= (GAME_STATE[SIDE]==6 ? 1 :-1 ) * temp;
            returnValue=moves.moves[i];
        }

        memcpy(GAME_STATE,tempGameState , sizeof(uint64_t)*24);
        memcpy(_chessBoard,tempBoard, sizeof(uint8_t)*64);
    }


    return returnValue;
}
