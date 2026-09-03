#include "../1. core/search.h"

#include <limits.h>
#include <stdint.h>
#include <string.h>

#define INF 10000000.0

typedef struct moveValue {
    uint16_t move;
    double evaluation;
} moveValue;



static void sortMoves(MoveList *moves, int maxNode)
{
    if (moves->index <= 1)
        return;

    moveValue scoredMoves[moves->index];

    uint64_t tempGameState[24];
    uint8_t tempBoard[64];

    for (int i = 0; i < moves->index; i++) {

        memcpy(tempGameState,
               GAME_STATE,
               sizeof(tempGameState));

        memcpy(tempBoard,
               _chessBoard,
               sizeof(tempBoard));

        makeMove(moves->moves[i]);

        scoredMoves[i].move = moves->moves[i];
        scoredMoves[i].evaluation = evaulateThisPosition();

        memcpy(GAME_STATE,
               tempGameState,
               sizeof(tempGameState));

        memcpy(_chessBoard,
               tempBoard,
               sizeof(tempBoard));
    }



    for (int i = 0; i < moves->index - 1; i++) {

        int bestIndex = i;

        for (int j = i + 1; j < moves->index; j++) {

            if (maxNode) {
                /*
                 * MAX:
                 * highest score first
                 */
                if (scoredMoves[j].evaluation >
                    scoredMoves[bestIndex].evaluation) {

                    bestIndex = j;
                }
            }
            else {
                /*
                 * MIN:
                 * lowest score first
                 */
                if (scoredMoves[j].evaluation <
                    scoredMoves[bestIndex].evaluation) {

                    bestIndex = j;
                }
            }
        }

        if (bestIndex != i) {

            moveValue temp = scoredMoves[i];

            scoredMoves[i] = scoredMoves[bestIndex];

            scoredMoves[bestIndex] = temp;
        }
    }


    /*
     * Put sorted moves back into MoveList.
     */
    for (int i = 0; i < moves->index; i++) {
        moves->moves[i] = scoredMoves[i].move;
    }
}


moveValue maxGame(int depth, double alpha, double beta);


moveValue minGame(int depth, double alpha, double beta)
{
    double evaluationValue = INF;

    MoveList moves;
    memset(&moves, 0, sizeof(MoveList));

    generateMoveList(&moves);

    moveValue temp1 = {};
    temp1.evaluation = evaluationValue;

    uint64_t tempGameState[24];
    uint8_t tempBoard[64];

    if (moves.index) {
        temp1.move = moves.moves[0];
    }


    if (depth == 1) {

        for (int i = 0; i < moves.index; i++) {

            memcpy(tempGameState,
                   GAME_STATE,
                   sizeof(tempGameState));

            memcpy(tempBoard,
                   _chessBoard,
                   sizeof(tempBoard));

            makeMove(moves.moves[i]);

            double temp = evaulateThisPosition();

            if (evaluationValue >= temp) {

                evaluationValue = temp;
                temp1.move = moves.moves[i];
            }


            memcpy(GAME_STATE,
                   tempGameState,
                   sizeof(tempGameState));

            memcpy(_chessBoard,
                   tempBoard,
                   sizeof(tempBoard));

            temp1.evaluation = evaluationValue;

            if (evaluationValue <= alpha) {
                return temp1;
            }
        }

        temp1.evaluation = evaluationValue;

        return temp1;
    }



    sortMoves(&moves, 0);


    for (int i = 0; i < moves.index; i++) {

        memcpy(tempGameState,
               GAME_STATE,
               sizeof(tempGameState));

        memcpy(tempBoard,
               _chessBoard,
               sizeof(tempBoard));

        makeMove(moves.moves[i]);

        moveValue _temp =
            maxGame(depth - 1,
                    alpha,
                    evaluationValue);


        if (evaluationValue >= _temp.evaluation) {

            evaluationValue = _temp.evaluation;

            temp1.move = moves.moves[i];
            temp1.evaluation = evaluationValue;
        }


        memcpy(GAME_STATE,
               tempGameState,
               sizeof(tempGameState));

        memcpy(_chessBoard,
               tempBoard,
               sizeof(tempBoard));


        if (evaluationValue <= alpha) {
            return temp1;
        }
    }

    return temp1;
}


moveValue maxGame(int depth, double alpha, double beta)
{
    double evaluationValue = -INF;

    MoveList moves;
    memset(&moves, 0, sizeof(MoveList));

    generateMoveList(&moves);

    moveValue temp1 = {};
    temp1.evaluation = evaluationValue;

    uint64_t tempGameState[24];
    uint8_t tempBoard[64];

    if (moves.index) {
        temp1.move = moves.moves[0];
    }


    if (depth == 1) {

        for (int i = 0; i < moves.index; i++) {

            memcpy(tempGameState,
                   GAME_STATE,
                   sizeof(tempGameState));

            memcpy(tempBoard,
                   _chessBoard,
                   sizeof(tempBoard));

            makeMove(moves.moves[i]);

            double temp = evaulateThisPosition();

            if (evaluationValue <= temp) {

                evaluationValue = temp;
                temp1.move = moves.moves[i];
            }

            memcpy(GAME_STATE,
                   tempGameState,
                   sizeof(tempGameState));

            memcpy(_chessBoard,
                   tempBoard,
                   sizeof(tempBoard));

            temp1.evaluation = evaluationValue;

            if (evaluationValue >= beta) {
                return temp1;
            }
        }

        temp1.evaluation = evaluationValue;

        return temp1;
    }



    sortMoves(&moves, 1);


    for (int i = 0; i < moves.index; i++) {

        memcpy(tempGameState,
               GAME_STATE,
               sizeof(tempGameState));

        memcpy(tempBoard,
               _chessBoard,
               sizeof(tempBoard));

        makeMove(moves.moves[i]);

        moveValue _temp =
            minGame(depth - 1,
                    evaluationValue,
                    beta);


        if (evaluationValue <= _temp.evaluation) {

            evaluationValue = _temp.evaluation;

            temp1.move = moves.moves[i];
            temp1.evaluation = evaluationValue;
        }



        memcpy(GAME_STATE,
               tempGameState,
               sizeof(tempGameState));

        memcpy(_chessBoard,
               tempBoard,
               sizeof(tempBoard));


        if (evaluationValue >= beta) {
            return temp1;
        }
    }

    return temp1;
}


uint16_t minmax_ALPHA_BETA(int depth)
{
    if (!GAME_STATE[SIDE]) {

        return maxGame(depth, -INF, INF).move;
    }

    return minGame(depth, -INF, INF).move;
}


uint16_t goSearch()
{
    return minmax_ALPHA_BETA(7); // 7 works fine 8 doesn't , i need to do incremental evaluation here , so i don't just recalculate everything from scratch
}
