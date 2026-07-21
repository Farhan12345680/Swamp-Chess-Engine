#include "Board Representation/bitboards.h";
#include "Board Representation/FEN_parser.h";
#include "Board Representation/gameHistory.h";



int main(){
    GameState _globalGame = initiaizeNewGame();

    printf("%s\n" ,fenStringGenerationFromGameState(&_globalGame) );

    

    

}