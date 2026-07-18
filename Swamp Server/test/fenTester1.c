#include "../Board Representation/FEN_parser.h"
#include <stdlib.h>

int main()
{

    FILE *fp;

    fp = fopen("fen.txt", "r");
    char *str = (char *)malloc(sizeof(char) * MAX_UCI_QUERY_LENGTH);
    while (fgets(str, MAX_UCI_QUERY_LENGTH, fp) != NULL)
    {
        if (fenStringValidator(str))
        {
            printf("FEN IS CORRECT\n");
        }
        else
        {
            printf("FEN IS WRONG\n");
        }
    }

    free(str);
    fclose(fp);
}