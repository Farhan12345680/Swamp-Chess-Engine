#include "./Board Representation/BoardRepresentation.h";
#include <stdio.h>;


    // DO initialization

    // Server Reading

int main(){
    GameState _globalGame = initiaizeNewGame();
    char * str = fenStringGenerationFromGameState(&_globalGame);
    printf("%s\n" ,str );
    free(str);
    

    

        if (strcmp(_str, "uci") == 0)
        {
        }
        else
        {
            free(_str);
            break;
        }
    }
