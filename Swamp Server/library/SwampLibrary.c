#include "../1. core/SwampLibrary.h"
#include <stdbool.h>






void init(){
    initializer();
};


void initFromFen(char* fenString){
    int ret=initializeNewGameFromString(fenString);
    if(ret==-1){
        printf("Wrong fen\n");
        initializer();
    }
};


void doMove(uint16_t move){
    makeMove(move);
};

char* goSearchNextBestMove(){
    uint16_t move = goSearch();

    static char movearr[6];

    uint16_t src  = move & 63;
    move >>= 6;
    uint16_t dest = move & 63;
    move >>=6;
    uint16_t promotion = move ;
    char promotionPie[5] = {'\0', 'q', 'r', 'b', 'n'};
    movearr[0]=(char)((src%8) + 'a');
    movearr[1]=(char)((src/8) + '1');
    movearr[2]=(char)((dest%8) + 'a');
    movearr[3]=(char)((dest/8) + '1');

    movearr[4] = promotionPie[promotion];
    movearr[5] = '\0';

    return movearr;
};


bool isValidMove(uint16_t move){
    MoveList moves;
    memset(&moves, 0, sizeof(MoveList));
    generateMoveList(&moves);

    for(int i=0;i<moves.index; i++){
        if(moves.moves[i] == move){
            return true;
        }
    }

    return false;
};


char* getFenStringFromGame(){
    return getFenFromBoard();
}
