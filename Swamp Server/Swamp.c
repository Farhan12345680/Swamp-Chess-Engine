#include "./Board Representation/BoardRepresentation.h";
#include "./Move Generation/MoveGeneration.h";
#include "./Move Generation/PieceMovement.h";
#include <string.h>;
#include <stdio.h>;


    // DO initialization

    // Server Reading

int main(){
    GameState _globalGame = initiaizeNewGame();
    char * _str = fenStringGenerationFromGameState(&_globalGame);
    printf("%s\n" ,_str );
    free(_str);
    

    

        if (strcmp(_str, "uci") == 0)
        {
        }
        else
        {
            free(_str);
        }
    }
