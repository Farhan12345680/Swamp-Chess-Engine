#pragma once
#include "./GameStateMoveGeneration.h"
#include <string.h>

#define SET_BIT_AT(rank, file) \
    ((uint64_t)1ULL << (((rank) - 1) * 8 + (file)))

#define SET_BIT_PIECE(piece, rank, file) \
    do { \
        (piece) |= SET_BIT_AT((rank), (file)); \
        (file)++; \
    } while (0)

typedef enum
{
    PIECE_PLACEMENT,
    SIDE_TO_MOVE,
    CASTLING_ABILITY,
    ENPASSANT_TARGET,
    HALF_MOVE,
    FULL_MOVE,
    DONE
} FEN_STATE;

typedef struct FEN_STRING
{
    char piecePlacement[100];
    char sideTOMove;
    char castlingAbility[10];
    char enpassantTargetSquare[5];
    char halfmoveClock[10];
    char fullMoveClock[10];

} FEN_STRING;



#define UCI_LINE_SIZE 4096


int initializeNewGameFromString(char *string)
{
    FEN_STRING _string1;

    char copy[UCI_LINE_SIZE];
    memset(copy , 0 , UCI_LINE_SIZE);
    memcpy(copy , string , strlen(string));
    int length[]={100 , 1 , 4 , 2 , 10 , 10 };
    copy[strcspn(copy , "\r\n")]='\0';
    char* token = strtok(copy , " \t\r\n");

    for(int i=0;i<6 ; i++){

        if(token==NULL){
            printf("problem 1");
            printf("%s\n",string);
            return -1;
        }
        switch (i)
        {
        case 0:
            memcpy(_string1.piecePlacement,token , strlen(token)+1);
            break;
        case 1:
            memcpy(&(_string1.sideTOMove) , token , strlen(token));
            break;
        case 2:
            memcpy(_string1.castlingAbility , token , strlen(token)+1);
            break;
        case 3:
            memcpy(_string1.enpassantTargetSquare , token , strlen(token)+1);
            break;
        case 4:
            memcpy(_string1.halfmoveClock , token , strlen(token)+1);
            break;
        case 5:
            memcpy(_string1.fullMoveClock , token , strlen(token)+1);
            break;
        default:
            break;
        }
        token=strtok(NULL , " \t\r\n" );
    }

    // printf("%s\n%c\n%s\n%s\n%s\n%s\n",_string1.piecePlacement ,_string1.sideTOMove,_string1.castlingAbility ,_string1.enpassantTargetSquare,_string1.halfmoveClock ,_string1.fullMoveClock);


    _globalZorbistHashing = generateZorbistNumbers();

    GAME_STATE[EVALUATION] = 1000;

    GAME_STATE[BLACK_PAWN_OCCUPANCY] = 0;
    GAME_STATE[WHITE_PAWN_OCCUPANCY] = 0;
    GAME_STATE[BLACK_ROOK_OCCUPANCY] = 0;
    GAME_STATE[BLACK_KNIGHT_OCCUPANCY] = 0;
    GAME_STATE[BLACK_BISHOP_OCCUPANCY] = 0;
    GAME_STATE[BLACK_KING_OCCUPANCY] = 0;
    GAME_STATE[BLACK_QUEEN_OCCUPANCY] = 0;
    GAME_STATE[WHITE_ROOK_OCCUPANCY] = 0;
    GAME_STATE[WHITE_KNIGHT_OCCUPANCY] = 0;
    GAME_STATE[WHITE_BISHOP_OCCUPANCY] = 0;
    GAME_STATE[WHITE_QUEEN_OCCUPANCY] = 0;
    GAME_STATE[WHITE_KING_OCCUPANCY] = 0;

    GAME_STATE[CASTLING_ABILITY] = 0;
    GAME_STATE[SIDE] = 0;
    GAME_STATE[ZORBIST_HASH] = 0;
    GAME_STATE[ENPASSANT_SQUARE] = NS;
    GAME_STATE[NUMBER_FULL_MOVES] = 1;
    GAME_STATE[NUMBER_HALF_MOVES] = 0;

    int _rankCounter = 8;
    int _currPosition = 0;
    int pos = 0;

    while (pos < strlen(_string1.piecePlacement))
    {


        switch (_string1.piecePlacement[pos])
        {
        case '/':
            _rankCounter--;
            if(_currPosition!=8){
                printf("problem 2");

                return -1;
            }
            _currPosition = 0;
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
            _currPosition += (_string1.piecePlacement[pos] - '0');

            break;
        case 'k':
            SET_BIT_PIECE(GAME_STATE[BLACK_KING_OCCUPANCY], _rankCounter, _currPosition);
            break;
        case 'K':
            SET_BIT_PIECE(GAME_STATE[WHITE_KING_OCCUPANCY], _rankCounter, _currPosition);
            break;
        case 'q':
            SET_BIT_PIECE(GAME_STATE[BLACK_QUEEN_OCCUPANCY], _rankCounter, _currPosition);

            break;
        case 'Q':
            SET_BIT_PIECE(GAME_STATE[WHITE_QUEEN_OCCUPANCY], _rankCounter, _currPosition);

            break;
        case 'p':
            SET_BIT_PIECE(GAME_STATE[BLACK_PAWN_OCCUPANCY], _rankCounter, _currPosition);

            break;
        case 'P':
            SET_BIT_PIECE(GAME_STATE[WHITE_PAWN_OCCUPANCY], _rankCounter, _currPosition);

            break;
        case 'b':
            SET_BIT_PIECE(GAME_STATE[BLACK_BISHOP_OCCUPANCY], _rankCounter, _currPosition);

            break;
        case 'B':
            SET_BIT_PIECE(GAME_STATE[WHITE_BISHOP_OCCUPANCY], _rankCounter, _currPosition);


            break;
        case 'n':
            SET_BIT_PIECE(GAME_STATE[BLACK_KNIGHT_OCCUPANCY], _rankCounter, _currPosition);

            break;
        case 'N':
            SET_BIT_PIECE(GAME_STATE[WHITE_KNIGHT_OCCUPANCY], _rankCounter, _currPosition);


            break;
        case 'r':
            SET_BIT_PIECE(GAME_STATE[BLACK_ROOK_OCCUPANCY], _rankCounter, _currPosition);


            break;
        case 'R':
            SET_BIT_PIECE(GAME_STATE[WHITE_ROOK_OCCUPANCY], _rankCounter, _currPosition);
            break;
        default:
            return -1;
        }

        pos++;

    }
    if(_currPosition!=8 && _rankCounter!=8){
                    printf("problem 3");

        return -1;
    }
    GAME_STATE[TOTAL_OCCUPANCY] =
        GAME_STATE[BLACK_PAWN_OCCUPANCY] | GAME_STATE[BLACK_ROOK_OCCUPANCY] |
        GAME_STATE[BLACK_KNIGHT_OCCUPANCY] | GAME_STATE[BLACK_BISHOP_OCCUPANCY] |
        GAME_STATE[BLACK_QUEEN_OCCUPANCY] | GAME_STATE[BLACK_KING_OCCUPANCY] |
        GAME_STATE[WHITE_PAWN_OCCUPANCY] | GAME_STATE[WHITE_ROOK_OCCUPANCY] |
        GAME_STATE[WHITE_KNIGHT_OCCUPANCY] | GAME_STATE[WHITE_QUEEN_OCCUPANCY] |
        GAME_STATE[WHITE_KING_OCCUPANCY] | GAME_STATE[WHITE_BISHOP_OCCUPANCY];

    GAME_STATE[BLACK_OCCUPANCY] =
        GAME_STATE[BLACK_PAWN_OCCUPANCY] | GAME_STATE[BLACK_ROOK_OCCUPANCY] |
        GAME_STATE[BLACK_KNIGHT_OCCUPANCY] | GAME_STATE[BLACK_BISHOP_OCCUPANCY] |
        GAME_STATE[BLACK_QUEEN_OCCUPANCY] | GAME_STATE[BLACK_KING_OCCUPANCY];

    GAME_STATE[WHITE_OCCUPANCY] = GAME_STATE[TOTAL_OCCUPANCY] ^ GAME_STATE[BLACK_OCCUPANCY];


    if(_string1.sideTOMove !='w' && _string1.sideTOMove !='b' )
    {
                    printf("problem 4");
        return -1;

    }


    GAME_STATE[SIDE] = (_string1.sideTOMove == 'w') ? 0 : 6;



    if (strcmp(_string1.castlingAbility , "-")==0)
    {
        GAME_STATE[CASTLING_AVAILABLE] = 0;
    }
    else
    {
            GAME_STATE[CASTLING_AVAILABLE] = 0;

            if (strcmp(_string1.castlingAbility, "-") != 0)
            {
                for (int i = 0; _string1.castlingAbility[i] != '\0'; i++)
                {
                    switch (_string1.castlingAbility[i])
                    {
                        case 'K':
                            GAME_STATE[CASTLING_AVAILABLE] |= 1 << 3;
                            break;

                        case 'Q':
                            GAME_STATE[CASTLING_AVAILABLE] |= 1 << 2;
                            break;

                        case 'k':
                            GAME_STATE[CASTLING_AVAILABLE] |= 1 << 1;
                            break;

                        case 'q':
                            GAME_STATE[CASTLING_AVAILABLE] |= 1 << 0;
                            break;

                        default:
                            return -1;
                    }
                }
            }
    }


    if (strcmp(_string1.enpassantTargetSquare ,"-")==0)
    {
        GAME_STATE[ENPASSANT_SQUARE] = NS;

    }
    else
    {
        if(_string1.enpassantTargetSquare[0]<='a' && _string1.enpassantTargetSquare[0]>='h'){
            return -1;
        }
        if(_string1.enpassantTargetSquare[1]<='1' || _string1.enpassantTargetSquare[1]>='8'){
            return -1;
        }
        int file = _string1.enpassantTargetSquare[0] - 'a';
        int rank = _string1.enpassantTargetSquare[1] - '1';
        GAME_STATE[ENPASSANT_SQUARE] = (Square)(file + rank * 8);
    }

    char *end;
    long value = strtol(_string1.halfmoveClock, &end, 10);

    if (end == _string1.halfmoveClock) {
        printf("problem 5");
        return -1;
    }

    if (*end != '\0') {
                printf("problem 6");

        return -1;
    }

    GAME_STATE[NUMBER_HALF_MOVES] = value;



    value = strtol(_string1.fullMoveClock, &end, 10);

    if (end == _string1.fullMoveClock) {
                printf("problem 7");

        return -1;
    }

    if (*end != '\0') {
                printf("problem 8");

        return -1;
    }
    if(!(__builtin_popcountll(GAME_STATE[BLACK_KING_OCCUPANCY])==1)){
                printf("problem 9");

        return -1;
    }
    if(!(__builtin_popcountll(GAME_STATE[WHITE_KING_OCCUPANCY])==1)){
                       printf("problem 10");

        return -1;
    }
    GAME_STATE[NUMBER_FULL_MOVES] = value;
    GAME_STATE[ZORBIST_HASH] = generateZorbistHashFromAGameState();

    initializeHelperFunc();

    return 1;
}
