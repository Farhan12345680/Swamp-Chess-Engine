#include "Common.h"
#include "bitboards.h"

typedef enum
{
    PIECE_PLACEMENT,
    SIDE_TO_MOVE,
    CASTLING_ABILITY,
    ENPASSANT_TARGET,
    HALF_MOVE,
    FULL_MOVE
} FEN_STATE;

int *returnPiecePlacementArrayCounter()
{

    int *arr = (int *)malloc(sizeof(char) * 128 * sizeof(int));

    for (int i = 0; i < 128; i++)
    {
        if (((char)i) == 'P' || ((char)i) == 'p')
        {
            arr[i] = 8;
        }
        else if (((char)i) == 'R' || ((char)i) == 'r' ||
                 ((char)i) == 'b' || ((char)i) == 'B' ||
                 ((char)i) == 'N' || ((char)i) == 'n' ||
                 ((char)i) == 'Q' || ((char)i) == 'q')
        {
            arr[i] = 9;
        }
        else if (((char)i) == 'K' || ((char)i) == 'k')
        {
            arr[i] = 1;
        }
        else if (((char)i) == '1' || ((char)i) == '2' ||
                 ((char)i) == '3' || ((char)i) == '4' ||
                 ((char)i) == '5' || ((char)i) == '6' ||
                 ((char)i) == '7')
        {
            arr[i] = 16;
        }
        else if (((char)i) == '8')
        {
            arr[i] = 8;
        }
        else
        {
            arr[i] = 0;
        }
    }

    return &arr[0];
}

bool fenStringValidator(char *FEN_STRING)
{
    FEN_STATE _state = PIECE_PLACEMENT;

    for (int i = 0; i < MAX_UCI_QUERY_LENGTH && FEN_STRING[i] != '\0'; i++)
    {
        if (_state == PIECE_PLACEMENT)
        {
            int *_asciiArray = returnPiecePlacementArrayCounter();

            int _rankPieceCounter = 0;
            int _slashCounter = 0;
            bool _wasLastCharacterADigit = false;

            while (_slashCounter < 8)
            {
                if (FEN_STRING[i] == '/')
                {
                    if (_rankPieceCounter != 8)
                    {
                        return false;
                    }
                    _wasLastCharacterADigit = false;

                    i++;
                    _slashCounter += 1;
                    _rankPieceCounter = 0;
                }
                else if (FEN_STRING[i] == ' ')
                {
                    i++;
                    _wasLastCharacterADigit = 1;

                    _state = SIDE_TO_MOVE;
                    break;
                }
                else
                {

                    int _val = FEN_STRING[i] - 0;
                    if (_asciiArray[_val]-- == 0)
                    {
                        return false;
                    }

                    if ((((char)FEN_STRING[i]) == '1' || ((char)FEN_STRING[i]) == '2' ||
                         ((char)FEN_STRING[i]) == '3' || ((char)FEN_STRING[i]) == '4' ||
                         ((char)FEN_STRING[i]) == '5' || ((char)FEN_STRING[i]) == '6' ||
                         ((char)FEN_STRING[i]) == '7' || ((char)FEN_STRING[i]) == '8') &&
                        !_wasLastCharacterADigit)
                    {
                        _rankPieceCounter += (FEN_STRING[i] - '0') - 0;
                        _wasLastCharacterADigit = true;
                    }
                    else if ((((char)FEN_STRING[i]) == '1' || ((char)FEN_STRING[i]) == '2' ||
                              ((char)FEN_STRING[i]) == '3' || ((char)FEN_STRING[i]) == '4' ||
                              ((char)FEN_STRING[i]) == '5' || ((char)FEN_STRING[i]) == '6' ||
                              ((char)FEN_STRING[i]) == '7' || ((char)FEN_STRING[i]) == '8') &&
                             _wasLastCharacterADigit)
                    {
                        return false;
                    }
                    else
                    {
                        _rankPieceCounter++;
                        _wasLastCharacterADigit = false;
                    }

                    i++;
                }
            }
            continue;
        }

        return true;
    }
}

GameState initializeNewGameFromString(char *FEN_STRING)
{
}
