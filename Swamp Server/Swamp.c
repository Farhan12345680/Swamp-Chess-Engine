#include "Board Representation/bitboards.h";




int main(){
    GameState _globalGame = initiaizeNewGame();

    printBoard(_globalGame);

    printf("the number of black pawns %d \n" , __builtin_popcountll(_globalGame._blackPawns));

}