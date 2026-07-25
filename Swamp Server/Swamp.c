#include "Board Representation/bitboards.h";
#include "Board Representation/FEN_parser.h";
#include "Board Representation/gameHistory.h";



int main(){
    GameState _globalGame = initiaizeNewGame();
    char * str = fenStringGenerationFromGameState(&_globalGame);
    printf("%s\n" ,str );
    free(str);
    

    

}