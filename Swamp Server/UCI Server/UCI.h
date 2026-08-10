#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>

#include "../Board Representation/GameStateMoveGeneration.h"

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
    CMD_UNKNOWN

} UCI_COMMAND;



typedef struct
{
    bool perft;
    int depth;

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




static void resetGoCommand(GO_COMMAND *go)
{
    memset(go, 0, sizeof(GO_COMMAND));
}

static void resetPositionCommand(POSITION_COMMAND *position)
{
    memset(position, 0, sizeof(POSITION_COMMAND));
}




static bool parseGo(char *line, GO_COMMAND *go)
{
    resetGoCommand(go);

    char *token = strtok(line, " \t\r\n");

    if (token == NULL)
        return false;

    if (strcasecmp(token, "go") != 0)
        return false;


    while ((token = strtok(NULL, " \t\r\n")) != NULL)
    {

        if (strcasecmp(token, "depth") == 0)
        {
            token = strtok(NULL, " \t\r\n");

            if (token == NULL)
                return false;

            go->depth = true;
            go->depthValue = atoi(token);
        }

        else if (strcasecmp(token, "movetime") == 0)
        {
            token = strtok(NULL, " \t\r\n");

            if (token == NULL)
                return false;

            go->movetime = true;
            go->movetimeValue = strtoull(token, NULL, 10);
        }

        else if (strcasecmp(token, "wtime") == 0)
        {
            token = strtok(NULL, " \t\r\n");

            if (token == NULL)
                return false;

            go->wtime = true;
            go->wtimeValue = strtoull(token, NULL, 10);
        }


        else if (strcasecmp(token, "btime") == 0)
        {
            token = strtok(NULL, " \t\r\n");

            if (token == NULL)
                return false;

            go->btime = true;
            go->btimeValue = strtoull(token, NULL, 10);
        }




        else if (strcasecmp(token, "winc") == 0)
        {
            token = strtok(NULL, " \t\r\n");

            if (token == NULL)
                return false;

            go->winc = true;
            go->wincValue = strtoull(token, NULL, 10);
        }


        else if (strcasecmp(token, "binc") == 0)
        {
            token = strtok(NULL, " \t\r\n");

            if (token == NULL)
                return false;

            go->binc = true;
            go->bincValue = strtoull(token, NULL, 10);
        }

    }

    return true;
}

static bool parsePosition(char *line, POSITION_COMMAND *position)
{
    resetPositionCommand(position);

    char *token = strtok(line, " \t\r\n");

    if (token == NULL)
        return false;

    if (strcasecmp(token, "position") != 0)
        return false;


    // ========================================================
    // STARTPOS
    // ========================================================

    token = strtok(NULL, " \t\r\n");

    if (token == NULL)
        return false;


    if (strcasecmp(token, "startpos") == 0)
    {
        position->startpos = true;

        token = strtok(NULL, " \t\r\n");
    }


    else if (strcasecmp(token, "fen") == 0)
    {
        position->hasFen = true;

        position->fen[0] = '\0';

        /*
         * FEN has exactly 6 fields:
         *
         * 1. Piece placement
         * 2. Side to move
         * 3. Castling rights
         * 4. En passant square
         * 5. Halfmove clock
         * 6. Fullmove number
         */

        for (int i = 0; i < 6; i++)
        {
            token = strtok(NULL, " \t\r\n");

            if (token == NULL)
                return false;

            if (i != 0)
                strcat(position->fen, " ");

            strcat(position->fen, token);
        }

        token = strtok(NULL, " \t\r\n");
    }

    else
    {
        return false;
    }


    // ========================================================
    // MOVES
    // ========================================================

    if (token != NULL && strcasecmp(token, "moves") == 0)
    {
        token = strtok(NULL, " \t\r\n");

        while (token != NULL)
        {
            if (position->moveCount >= MAX_MOVES)
                return false;

            /*
             * Normal UCI move is 4 or 5 characters:
             *
             * e2e4
             * e7e8q
             */

            size_t length = strlen(token);

            if (length < 4 || length > 5)
                return false;

            strcpy(
                position->moves[position->moveCount],
                token
            );

            position->moveCount++;

            token = strtok(NULL, " \t\r\n");
        }
    }


    return true;
}


static bool parseUCI(char *line, UCI_COMMAND_DATA *command)
{
    memset(command, 0, sizeof(UCI_COMMAND_DATA));


    char copy[UCI_LINE_SIZE];

    strncpy(copy, line, UCI_LINE_SIZE - 1);
    copy[UCI_LINE_SIZE - 1] = '\0';

    char *token = strtok(copy, " \t\r\n");

    if (token == NULL)
    {
        return false;
    }       
    


    if (strcasecmp(token, "uci") == 0)
    {
        command->type = CMD_UCI;
        return true;
    }



    if (strcasecmp(token, "isready") == 0)
    {
        command->type = CMD_ISREADY;
        return true;
    }




    if (strcasecmp(token, "ucinewgame") == 0)
    {
        command->type = CMD_UCINEWGAME;
        return true;
    }


    if (strcasecmp(token, "stop") == 0)
    {
        command->type = CMD_STOP;
        return true;
    }



    if (strcasecmp(token, "quit") == 0)
    {
        command->type = CMD_QUIT;
        return true;
    }

    if (strcasecmp(token, "position") == 0)
    {
        command->type = CMD_POSITION;

        return parsePosition(line, &command->position);
    }




    if (strcasecmp(token, "go") == 0)
    {
        command->type = CMD_GO;

        return parseGo(line, &command->go);
    }


    command->type = CMD_UNKNOWN;

    return false;
}

void UCI_SERVER(void)
{
    char line[UCI_LINE_SIZE];

    UCI_COMMAND_DATA command;


    while (fgets(line, sizeof(line), stdin))
    {
        if (!parseUCI(line, &command))
        {
            continue;
        }


        switch (command.type)
        {

            case CMD_UCI:

                printf("id name Swamp\n");
                printf("id author Farhan Adib\n");
                printf("uciok\n");
                fflush(stdout);

                break;

            case CMD_ISREADY:

                printf("readyok\n");
                fflush(stdout);

                break;



            case CMD_UCINEWGAME:


                break;


            case CMD_POSITION:

                if (command.position.startpos)
                {
                }


                else if (command.position.hasFen)
                {

                }


                for (int i = 0; i < command.position.moveCount; i++)
                {

                }

                break;

            case CMD_GO:

                //printGoCommand(&command.go);


                /*
                 * Example:
                 *
                 * if (command.go.depth)
                 * {
                 *     searchDepth(command.go.depthValue);
                 * }
                 *
                 * else if (command.go.movetime)
                 * {
                 *     searchTime(command.go.movetimeValue);
                 * }
                 *
                 * else if (command.go.wtime ||
                 *          command.go.btime)
                 * {
                 *     searchWithClock(...);
                 * }
                 */

                break;


            case CMD_STOP:
                break;
            case CMD_QUIT:

                return;

            default:

                break;
        }
    }
}