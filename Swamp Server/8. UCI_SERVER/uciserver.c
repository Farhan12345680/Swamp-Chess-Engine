#include "../1. core/uciserver.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char globalPositionMoveOrder[256][6]={0};
int positionCount=0;

void resetGoCommand(GO_COMMAND *go)
{
    memset(go, 0, sizeof(GO_COMMAND));
}

void resetPositionCommand(POSITION_COMMAND *position)
{
    memset(position, 0, sizeof(POSITION_COMMAND));
}

bool parseGo(char *line, GO_COMMAND *go)
{
    resetGoCommand(go);
    char *saveptr;
    char *token = strtok_r(line, " \t\r\n", &saveptr);
    if (token == NULL)
        return false;
    if (strcasecmp(token, "go") != 0)
        return false;

    while ((token = strtok_r(NULL, " \t\r\n", &saveptr)) != NULL)
    {
        if (strcasecmp(token, "depth") == 0)
        {
            token = strtok_r(NULL, " \t\r\n", &saveptr);
            if (token == NULL)
                return false;
            go->depth = true;
            go->depthValue = strtoull(token, NULL, 10);
        }
        else if(strcasecmp(token , "perft")==0)
        {
            token = strtok_r(NULL, " \t\r\n", &saveptr);
            if (token == NULL)
                return false;
            go->perft = true;
            go->perft_level = strtoull(token, NULL, 10);
        }
        else if (strcasecmp(token, "movetime") == 0)
        {
            token = strtok_r(NULL, " \t\r\n", &saveptr);
            if (token == NULL)
                return false;
            go->movetime = true;
            go->movetimeValue = strtoull(token, NULL, 10);
        }
        else if (strcasecmp(token, "wtime") == 0)
        {
            token = strtok_r(NULL, " \t\r\n", &saveptr);
            if (token == NULL)
                return false;
            go->wtime = true;
            go->wtimeValue = strtoull(token, NULL, 10);
        }
        else if (strcasecmp(token, "btime") == 0)
        {
            token = strtok_r(NULL, " \t\r\n", &saveptr);
            if (token == NULL)
                return false;
            go->btime = true;
            go->btimeValue = strtoull(token, NULL, 10);
        }
        else if (strcasecmp(token, "winc") == 0)
        {
            token = strtok_r(NULL, " \t\r\n", &saveptr);
            if (token == NULL)
                return false;
            go->winc = true;
            go->wincValue = strtoull(token, NULL, 10);
        }
        else if (strcasecmp(token, "binc") == 0)
        {
            token = strtok_r(NULL, " \t\r\n", &saveptr);
            if (token == NULL)
                return false;
            go->binc = true;
            go->bincValue = strtoull(token, NULL, 10);
        }
    }
    return true;
}

bool parsePosition(char *line, POSITION_COMMAND *position)
{
    resetPositionCommand(position);
    char *saveptr;
    char *token = strtok_r(line, " \t\r\n", &saveptr);
    if (!token || strcasecmp(token, "position") != 0)
        return false;

    token = strtok_r(NULL, " \t\r\n", &saveptr);
    if (!token) return false;

    if (strcasecmp(token, "startpos") == 0)
    {
        position->startpos = true;
    }
    else if (strcasecmp(token, "fen") == 0)
    {
        position->hasFen = true;
        position->fen[0] = '\0';
        for (int i = 0; i < 6; i++)
        {
            token = strtok_r(NULL, " \t\r\n", &saveptr);
            if (!token) return false;
            strcat(position->fen, token);
            strcat(position->fen, " ");
        }
    }
    else
    {
        return false;
    }

    token = strtok_r(NULL, " \t\r\n", &saveptr);
    if (token && strcasecmp(token, "moves") == 0)
    {
        while ((token = strtok_r(NULL, " \t\r\n", &saveptr)) != NULL)
        {
            if (position->moveCount >= MAX_MOVES) break;
            size_t len = strlen(token);
            if (len < 4 || len > 5) return false;
            strcpy(position->moves[position->moveCount++], token);
        }
    }

    return true;
}

bool parseUCI(char *line, UCI_COMMAND_DATA *command)
{
    memset(command, 0, sizeof(UCI_COMMAND_DATA));

    char copy[UCI_LINE_SIZE];
    strncpy(copy, line, UCI_LINE_SIZE - 1);
    copy[UCI_LINE_SIZE - 1] = '\0';

    char *saveptr;
    char *token = strtok_r(copy, " \t\r\n", &saveptr);

    if (token == NULL)
    {
        return false;
    }

    if (strcasecmp(token, "printboard") == 0)
    {
        command->type = CMD_PRINTBOARD;
        return true;
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

// void getMove(){

// }

void* UCI_SERVER(void* arg)
{
    pthread_detach(pthread_self());

    char line[UCI_LINE_SIZE];
    UCI_COMMAND_DATA command;

    while (fgets(line, sizeof(line), stdin))
    {
        if (!parseUCI(line, &command))
        {
            continue;
        }

        commandPoint* temp = (void * )malloc(sizeof(commandPoint));
        temp->next= NULL;

        switch (command.type)
        {
            case CMD_PRINTBOARD:
                pthread_mutex_lock(&inputLock);

                temp->CMD_CASE=CMD_TYPE_PRINTBOARD;

                if(!ququeSize)
                {
                    head =temp;
                    tail = head;
                }
                else
                {
                    tail->next =temp;
                    tail = temp;
                }

                ququeSize++;

                pthread_cond_broadcast(&inputCond);
                pthread_mutex_unlock(&inputLock);
                break;

            case CMD_UCI:
                printf("id name Swamp\n");
                printf("id author Farhan Adib\n");
                printf("uciok\n");
                fflush(stdout);
                break;

            case CMD_ISREADY:
                pthread_mutex_lock(&inputLock);

                temp->CMD_CASE=CMD_TYPE_ISREADY;
                if(!ququeSize){
                    head =temp;
                    tail = head;
                }else{
                    tail->next =temp;
                    tail = temp;
                }
                ququeSize++;

                pthread_cond_broadcast(&inputCond);
                pthread_mutex_unlock(&inputLock);
                break;

            case CMD_UCINEWGAME:
                pthread_mutex_lock(&inputLock);

                temp->CMD_CASE=CMD_TYPE_STARTPOS;
                if(!ququeSize){
                    head =temp;
                    tail = head;
                }else{
                    tail->next =temp;
                    tail = temp;
                }
                ququeSize++;

                pthread_cond_broadcast(&inputCond);
                pthread_mutex_unlock(&inputLock);
                break;

            case CMD_POSITION:
                if (command.position.hasFen)
                {
                    pthread_mutex_lock(&inputLock);

                    temp->CMD_CASE=CMD_TYPE_FENPOSITION;
                    if(!ququeSize){
                        head =temp;
                        tail = head;
                    }else{
                        tail->next =temp;
                        tail = temp;
                    }
                    ququeSize++;

                    memcpy(temp->fenString, command.position.fen, strlen(command.position.fen)+1);

                    pthread_cond_broadcast(&inputCond);
                    pthread_mutex_unlock(&inputLock);
                    break;
                }

                pthread_mutex_lock(&inputLock);

                temp->CMD_CASE=CMD_TYPE_MAKEMOVE;
                if(!ququeSize){
                    head =temp;
                    tail = head;
                }else{
                    tail->next =temp;
                    tail = temp;
                }
                ququeSize++;

                for(int j=0; j<command.position.moveCount; j++)
                {
                    memcpy( temp->globalPositionMoveOrder[j],
                            command.position.moves[j],
                            strlen(command.position.moves[j])+1);
                }
                temp->moveCount= command.position.moveCount;

                pthread_cond_broadcast(&inputCond);
                pthread_mutex_unlock(&inputLock);
                break;

            case CMD_GO:
                if (command.go.perft)
                {
                    pthread_mutex_lock(&inputLock);

                    temp->CMD_CASE=CMD_TYPE_PERFT;
                    if(!ququeSize){
                        head =temp;
                        tail = head;
                    }else{
                        tail->next =temp;
                        tail = temp;
                    }
                    ququeSize++;

                    memcpy(&temp->perftDepth, &command.go.perft_level, sizeof(command.go.perft_level));


                    pthread_cond_broadcast(&inputCond);
                    pthread_mutex_unlock(&inputLock);
                    break;
                }

                if(command.go.wtime)
                {
                    temp->wtime= command.go.wtimeValue;
                }
                if(command.go.btime)
                {
                    temp->btime=command.go.btimeValue;
                }

                temp->CMD_CASE=CMD_TYPE_GO_SEARCH;

                if(!ququeSize){
                    head =temp;
                    tail = head;
                }else{
                    tail->next =temp;
                    tail = temp;
                }
                ququeSize++;

                pthread_cond_broadcast(&inputCond);
                pthread_mutex_unlock(&inputLock);

                break;

            case CMD_STOP:

                break;

            case CMD_QUIT:
                return (void*)1;

            default:
                break;
        }
    }
    return NULL;
}
