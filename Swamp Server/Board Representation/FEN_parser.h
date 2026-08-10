#pragma once
#include "./GameStateMoveGeneration.h"

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

#define UCI_LINE_SIZE 4096

void initializeNewGameFromString(char *FEN_STRING)
{
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

    while (pos < UCI_LINE_SIZE  &&
           FEN_STRING[pos] != '\0' &&
           FEN_STRING[pos] != '\n')
    {
        switch (FEN_STRING[pos])
        {
        case '/':
            _rankCounter--;
            _currPosition = 0;
            pos++;
            break;
        case ' ':
            pos++;
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
            _currPosition += (FEN_STRING[pos] - '0');
            pos++;
            break;
        case 'k':
            SET_BIT_PIECE(GAME_STATE[BLACK_KING_OCCUPANCY], _rankCounter, _currPosition);
            pos++;
            break;
        case 'K':
            SET_BIT_PIECE(GAME_STATE[WHITE_KING_OCCUPANCY], _rankCounter, _currPosition);
            pos++;
            break;
        case 'q':
            SET_BIT_PIECE(GAME_STATE[BLACK_QUEEN_OCCUPANCY], _rankCounter, _currPosition);
            pos++;
            break;
        case 'Q':
            SET_BIT_PIECE(GAME_STATE[WHITE_QUEEN_OCCUPANCY], _rankCounter, _currPosition);
            pos++;
            break;
        case 'p':
            SET_BIT_PIECE(GAME_STATE[BLACK_PAWN_OCCUPANCY], _rankCounter, _currPosition);
            pos++;
            break;
        case 'P':
            SET_BIT_PIECE(GAME_STATE[WHITE_PAWN_OCCUPANCY], _rankCounter, _currPosition);
            pos++;
            break;
        case 'b':
            SET_BIT_PIECE(GAME_STATE[BLACK_BISHOP_OCCUPANCY], _rankCounter, _currPosition);
            pos++;
            break;
        case 'B':
            SET_BIT_PIECE(GAME_STATE[WHITE_BISHOP_OCCUPANCY], _rankCounter, _currPosition);
            pos++;
            break;
        case 'n':
            SET_BIT_PIECE(GAME_STATE[BLACK_KNIGHT_OCCUPANCY], _rankCounter, _currPosition);
            pos++;
            break;
        case 'N':
            SET_BIT_PIECE(GAME_STATE[WHITE_KNIGHT_OCCUPANCY], _rankCounter, _currPosition);
            pos++;
            break;
        case 'r':
            SET_BIT_PIECE(GAME_STATE[BLACK_ROOK_OCCUPANCY], _rankCounter, _currPosition);
            pos++;
            break;
        case 'R':
            SET_BIT_PIECE(GAME_STATE[WHITE_ROOK_OCCUPANCY], _rankCounter, _currPosition);
            pos++;
            break;
        default:
            pos++;
            break;
        }
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

    while (FEN_STRING[pos] == ' ')
        pos++;

    GAME_STATE[SIDE] = (FEN_STRING[pos] == 'w') ? 0 : 6;
    pos++;

    while (FEN_STRING[pos] == ' ')
        pos++;

    if (FEN_STRING[pos] == '-')
    {
        GAME_STATE[CASTLING_ABILITY] = 0;
        pos++;
    }
    else
    {
        const char rights[] = "KQkq";
        int idx = 0;
        while (FEN_STRING[pos] != ' ' && idx < 4)
        {
            if (FEN_STRING[pos] == rights[idx])
            {
                GAME_STATE[CASTLING_ABILITY] |= (1 << (3 - idx));
                pos++;
                idx++;
            }
            else
            {
                idx++;
            }
        }
        while (FEN_STRING[pos] == ' ')
            pos++;
    }

    while (FEN_STRING[pos] == ' ')
        pos++;

    if (FEN_STRING[pos] == '-')
    {
        GAME_STATE[ENPASSANT_SQUARE] = ES;
        pos++;
    }
    else
    {
        int file = FEN_STRING[pos] - 'a';
        int rank = FEN_STRING[pos + 1] - '1';
        GAME_STATE[ENPASSANT_SQUARE] = (Square)(file + rank * 8);
        pos += 2;
    }

    while (FEN_STRING[pos] == ' ')
        pos++;

    GAME_STATE[NUMBER_HALF_MOVES] = 0;
    while (FEN_STRING[pos] >= '0' && FEN_STRING[pos] <= '9')
    {
        GAME_STATE[NUMBER_HALF_MOVES] = GAME_STATE[NUMBER_HALF_MOVES] * 10 + (FEN_STRING[pos] - '0');
        pos++;
    }

    while (FEN_STRING[pos] == ' ')
        pos++;

    GAME_STATE[NUMBER_FULL_MOVES] = 0;
    while (FEN_STRING[pos] >= '0' && FEN_STRING[pos] <= '9')
    {
        GAME_STATE[NUMBER_FULL_MOVES] = GAME_STATE[NUMBER_FULL_MOVES] * 10 + (FEN_STRING[pos] - '0');
        pos++;
    }

    GAME_STATE[ZORBIST_HASH] = generateZorbistHashFromAGameState();

    initializeHelperFunc();
}