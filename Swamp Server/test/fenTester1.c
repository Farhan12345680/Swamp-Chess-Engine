#include "../Board Representation/FEN_parser.h"
#include <stdlib.h>

int main()
{

    FILE *fp;

    fp = fopen("fen.txt", "r");
    char *str = (char *)malloc(sizeof(char) * MAX_UCI_QUERY_LENGTH);
    while (fgets(str, MAX_UCI_QUERY_LENGTH, fp) != NULL)
    {
        str[strcspn(str, "\r")] = '\0';
        if (fenStringValidator(str))
        {
            GameState _g=initializeNewGameFromString(str);
            char *returned = fenStringGenerationFromGameState(_g);

            printf("CORRECT\nGIVEN -> %sReturned -> %s\n" , str,returned );
            free(returned);

        }
        else
        {

        }
    }

    free(str);
    fclose(fp);
}