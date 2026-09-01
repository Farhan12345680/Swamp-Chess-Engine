#include "../1. core/search.h"

#include <limits.h>
#include <stdint.h>
#include <string.h>

//basic minmax algorithm

typedef struct moveValue{
    uint16_t move;
    double evaluation;
} moveValue;


moveValue maxGame(int depth);

moveValue minGame(int depth){

    double evaluationValue = 10000000;

    MoveList moves;
    memset(&moves, 0, sizeof(MoveList));
    generateMoveList(&moves);
    moveValue temp1 = {};
    memset(&temp1, 0, sizeof(moveValue));
    temp1.evaluation=evaluationValue;

    uint64_t tempGameState[24];
    uint8_t  tempBoard[64];
    if(moves.index){
        temp1.move=moves.moves[0];
    }
    if(depth ==1){

        for (int i=0 ;i< moves.index; i++ ){
            memcpy(tempGameState, GAME_STATE , sizeof(uint64_t)*24);
            memcpy(tempBoard, _chessBoard, sizeof(uint8_t)*64);
            makeMove(moves.moves[i]);
            double temp = evaulateThisPosition();

            if( (evaluationValue >= temp)){
                evaluationValue=  temp;
                temp1.move=moves.moves[i];

            }

            memcpy(GAME_STATE,tempGameState , sizeof(uint64_t)*24);
            memcpy(_chessBoard,tempBoard, sizeof(uint8_t)*64);
        }
        temp1.evaluation= evaluationValue;
        return temp1;
    }

    for (int i=0 ;i< moves.index; i++ ){
        memcpy(tempGameState, GAME_STATE , sizeof(uint64_t)*24);
        memcpy(tempBoard, _chessBoard, sizeof(uint8_t)*64);
        makeMove(moves.moves[i]);
        moveValue _temp = maxGame(depth-1);

        if( (evaluationValue >= _temp.evaluation)){
            evaluationValue=  _temp.evaluation;
            temp1.move=moves.moves[i];
            temp1.evaluation= evaluationValue;

        }

        memcpy(GAME_STATE,tempGameState , sizeof(uint64_t)*24);
        memcpy(_chessBoard,tempBoard, sizeof(uint8_t)*64);
    }
    return temp1;
}

moveValue maxGame(int depth){


    double evaluationValue = -10000000;

    MoveList moves;
    memset(&moves, 0, sizeof(MoveList));
    generateMoveList(&moves);
    moveValue temp1 = {};
    memset(&temp1, 0, sizeof(moveValue));

    temp1.evaluation=evaluationValue;

    uint64_t tempGameState[24];
    uint8_t  tempBoard[64];
    if(moves.index){
        temp1.move=moves.moves[0];
    }
    if(depth ==1){
        for (int i=0 ;i< moves.index; i++ ){
            memcpy(tempGameState, GAME_STATE , sizeof(uint64_t)*24);
            memcpy(tempBoard, _chessBoard, sizeof(uint8_t)*64);
            makeMove(moves.moves[i]);
            double temp = evaulateThisPosition();

            if( (evaluationValue <= temp)){
                evaluationValue=  temp;
                temp1.move=moves.moves[i];

            }

            memcpy(GAME_STATE,tempGameState , sizeof(uint64_t)*24);
            memcpy(_chessBoard,tempBoard, sizeof(uint8_t)*64);
        }
        temp1.evaluation= evaluationValue;
        return temp1;
    }

    for (int i=0 ;i< moves.index; i++ ){
        memcpy(tempGameState, GAME_STATE , sizeof(uint64_t)*24);
        memcpy(tempBoard, _chessBoard, sizeof(uint8_t)*64);
        makeMove(moves.moves[i]);
        moveValue _temp = minGame(depth-1);

        if( (evaluationValue <= _temp.evaluation)){
            evaluationValue=  _temp.evaluation;
            temp1.move=moves.moves[i];
            temp1.evaluation= evaluationValue;

        }

        memcpy(GAME_STATE,tempGameState , sizeof(uint64_t)*24);
        memcpy(_chessBoard,tempBoard, sizeof(uint8_t)*64);
    }
    return temp1;
}


uint16_t minmax(int depth){
    if(!GAME_STATE[SIDE]){

        return maxGame(depth).move;
    }

     return minGame(depth).move;
}



uint16_t goSearch()
{
    double evaluationValue = -100000;
    uint16_t returnValue=0xFFFF;
    MoveList moves;

    return returnValue =minmax(5);


}
