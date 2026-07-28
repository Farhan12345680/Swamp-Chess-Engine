#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../Board Representation/BoardRepresentation.h"
int main()
{
    FILE *file = fopen("./FENTesting/fen.txt", "r");

    if (file == NULL)
    {
        perror("Failed to open FEN file");
        return;
    }

    char fen[256];
    int lineNumber = 0;

    while (fgets(fen, sizeof(fen), file) != NULL)
    {
        lineNumber++;

        // Remove \n and \r\n
        fen[strcspn(fen, "\r\n")] = '\0';

        // Skip empty lines
        if (fen[0] == '\0')
        {
            continue;
        }

        if (fenStringValidator(fen))
        {
            printf("Line %d: VALID\n", lineNumber);
            printf("FEN: %s\n", fen);
        }
        else
        {
            printf("Line %d: INVALID\n", lineNumber);
            printf("FEN: %s\n", fen);
        }
    }

    fclose(file);
}