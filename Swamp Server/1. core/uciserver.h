#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include "locks.h"
#include "core.h"

#define UCI_LINE_SIZE 4096
#define MAX_MOVES 256
#define MAX_FEN_SIZE 128


typedef enum
{
    CMD_UCI,
    CMD_ISREADY,
    CMD_UCINEWGAME,
    CMD_POSITION,
    CMD_GO,
    CMD_STOP,
    CMD_QUIT,
    CMD_UNKNOWN,
    CMD_PRINTBOARD

} UCI_COMMAND;



typedef struct
{
    bool perft;
    int perft_level;

    bool depth;
    int depthValue;

    bool nodes;
    __uint64_t nodesValue;

    bool movetime;
    __uint64_t movetimeValue;

    __uint64_t mate;
    int mateValue;


    bool wtime;
    __uint64_t wtimeValue;

    bool btime;
    __uint64_t btimeValue;

    bool winc;
    __uint64_t wincValue;

    bool binc;
    __uint64_t bincValue;

} GO_COMMAND;



typedef struct
{
    bool startpos;


    bool hasFen;
    char fen[MAX_FEN_SIZE];

    char moves[MAX_MOVES][6];
    int moveCount;

} POSITION_COMMAND;


typedef struct
{
    UCI_COMMAND type;

    GO_COMMAND go;

    POSITION_COMMAND position;

} UCI_COMMAND_DATA;

void resetGoCommand(GO_COMMAND *go);
void resetPositionCommand(POSITION_COMMAND *position);
bool parseGo(char *line, GO_COMMAND *go);
bool parsePosition(char *line, POSITION_COMMAND *position);
bool parseUCI(char *line, UCI_COMMAND_DATA *command);
void* UCI_SERVER(void* arg);
