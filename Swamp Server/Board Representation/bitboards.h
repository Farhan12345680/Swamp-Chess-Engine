#pragma once;
#include "Common.h"

typedef struct
{
    __uint64_t _blackPawns;
    __uint64_t _blackRooks;
    __uint64_t _blackBishops;
    __uint64_t _blackKnights;
    __uint64_t _blackQueens;
    __uint64_t _blackKing;

    __uint64_t _whitePawns;
    __uint64_t _whiteRooks;
    __uint64_t _whiteKnights;
    __uint64_t _whiteBishops;
    __uint64_t _whiteQueens;
    __uint64_t _whiteKing;

    __uint64_t _blackOccupancy;
    __uint64_t _whiteOccupancy;
    __uint64_t _occupancy;

    __uint64_t _zobrist_hash;
    int _randomValues[12][64];

    __uint64_t *_prevStates;
    int _stateIndex;
    int _totalState;

} GameState;

typedef enum
{
    A1,
    A2,
    A3,
    A4,
    A5,
    A6,
    A7,
    A8,
    B1,
    B2,
    B3,
    B4,
    B5,
    B6,
    B7,
    B8,
    C1,
    C2,
    C3,
    C4,
    C5,
    C6,
    C7,
    C8,
    D1,
    D2,
    D3,
    D4,
    D5,
    D6,
    D7,
    D8,
    E1,
    E2,
    E3,
    E4,
    E5,
    E6,
    E7,
    E8,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    G1,
    G2,
    G3,
    G4,
    G5,
    G6,
    G7,
    G8,
    H1,
    H2,
    H3,
    H4,
    H5,
    H6,
    H7,
    H8
} Square;

typedef enum
{
    WHITE_KING,
    WHITE_QUEEN,
    WHITE_BISHOP,
    WHITE_KNIGHT,
    WHITE_ROOK,
    BLACK_KING,
    BLACK_QUEEN,
    BLACK_BISHOP,
    BLACK_KNIGHT,
    BLACK_ROOK
} Pieces;

GameState initiaizeNewGame()
{
    GameState _newState;
    _newState._blackPawns = 0b0000000011111111000000000000000000000000000000000000000000000000;
    _newState._whitePawns = 0b0000000000000000000000000000000000000000000000001111111100000000;
    _newState._blackRooks = 0b1000000100000000000000000000000000000000000000000000000000000000;
    _newState._blackKnights = 0b0100001000000000000000000000000000000000000000000000000000000000;
    _newState._blackBishops = 0b0010010000000000000000000000000000000000000000000000000000000000;
    _newState._blackKing = 0b0000100000000000000000000000000000000000000000000000000000000000;
    _newState._blackQueens = 0b0001000000000000000000000000000000000000000000000000000000000000;
    _newState._whiteRooks = 0b0000000000000000000000000000000000000000000000000000000010000001;
    _newState._whiteKnights = 0b0000000000000000000000000000000000000000000000000000000001000010;
    _newState._whiteBishops = 0b0000000000000000000000000000000000000000000000000000000000100100;
    _newState._whiteQueens = 0b0000000000000000000000000000000000000000000000000000000000010000;
    _newState._whiteKing = 0b0000000000000000000000000000000000000000000000000000000000001000;

    _newState._occupancy =
        _newState._blackPawns ^ _newState._blackRooks ^
        _newState._blackKnights ^ _newState._blackBishops ^ _newState._blackQueens ^
        _newState._blackKing ^ _newState._whitePawns ^
        _newState._whiteRooks ^ _newState._whiteKnights ^
        _newState._whiteQueens ^ _newState._whiteKing ^
        _newState._whiteBishops;

    _newState._blackOccupancy =
        _newState._blackPawns ^ _newState._blackRooks ^
        _newState._blackKnights ^ _newState._blackBishops ^ _newState._blackQueens ^
        _newState._blackKing;

    _newState._whiteOccupancy = _newState._whitePawns ^
                                _newState._whiteRooks ^ _newState._whiteKnights ^
                                _newState._whiteQueens ^ _newState._whiteKing ^
                                _newState._whiteBishops;

    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            _newState._randomValues[i][j] = rand();
        }
    }

    _newState._prevStates = (__uint64_t *)malloc(sizeof(__uint64_t) * 100);
    _newState._stateIndex = 0;
    _newState._totalState = 100;

    return _newState;
}

void insertStateIntoGameState(GameState GAME_STATE, __uint64_t VALUE)
{
    if (GAME_STATE._stateIndex < GAME_STATE._totalState / 2)
    {
        GAME_STATE._prevStates[GAME_STATE._stateIndex] = VALUE;
        GAME_STATE._stateIndex++;

        return;
    }
    __uint64_t *new_array = (__uint64_t *)malloc(sizeof(__uint64_t) * 2 * GAME_STATE._totalState);

    for (int i = 0; i <= GAME_STATE._stateIndex; i++)
    {
        new_array[i] = GAME_STATE._prevStates[i];
    }

    free(GAME_STATE._prevStates);
    GAME_STATE._prevStates = new_array;
}

GameState characterPuter(char _board[8][8], __uint64_t PIECE_NUMBER, char CHARACTER)
{
    int _counter = 0;
    __uint64_t _temp = PIECE_NUMBER;

    while (_temp)
    {
        int _mod = _temp % 256;
        int _secondCounter = 0;

        while (_mod)
        {
            if (_mod & 1)
            {
                _board[_counter][_secondCounter] = CHARACTER;
            }
            _mod >>= 1;
            _secondCounter++;
        }

        _temp >>= 8;
        _counter++;
    }
}

GameState printBoard(GameState GAME_STATE)
{
    char _board[8][8];

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            _board[i][j] = '.';
        }
    }

    // black pawns
    characterPuter(_board, GAME_STATE._blackPawns, 'p');
    // black rooks
    characterPuter(_board, GAME_STATE._blackRooks, 'r');
    // black bishops
    characterPuter(_board, GAME_STATE._blackBishops, 'b');
    // black knights
    characterPuter(_board, GAME_STATE._blackKnights, 'n');
    // black queens
    characterPuter(_board, GAME_STATE._blackQueens, 'q');
    // black king
    characterPuter(_board, GAME_STATE._blackKing, 'k');

    // white pawns
    characterPuter(_board, GAME_STATE._whitePawns, 'P');
    // white rooks
    characterPuter(_board, GAME_STATE._whiteRooks, 'R');
    // white bishops
    characterPuter(_board, GAME_STATE._whiteBishops, 'B');
    // white knights
    characterPuter(_board, GAME_STATE._whiteKnights, 'N');
    // white queens
    characterPuter(_board, GAME_STATE._whiteQueens, 'q');
    // white king
    characterPuter(_board, GAME_STATE._whiteKing, 'K');

    for (int i = 7; i >= 0; i--)
    {
        for (int j = 0; j < 8; j++)
        {
            printf("%c ", _board[i][j]);
        }
        printf("\n");
    }
}
