#include "../1. core/uciserver.h"


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
            go->depthValue = strtoull(token, NULL, 10);

        }

        else if(strcasecmp(token , "perft")==0){
            token = strtok(NULL, " \t\r\n");

            if (token == NULL)
                return false;

            go->perft = true;
            go->perft_level = strtoull(token, NULL, 10);
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


bool parsePosition(char *line, POSITION_COMMAND *position)
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

        for(int i=0;i<6; i++)
        {
            token = strtok(NULL, " \t\r\n");

            if (token == NULL)
                return false;

            strcat(position->fen , token);
            strcat(position->fen , " ");

            // token[strcspn(position->fen , "\0")]=' ';


        }


        position->fen[strlen(position->fen)]='\0';
        // for(int i=0;i<60;i++){
        //     printf("-->%d %c\n",i, position->fen[i]);
        // }
        // printf("token i-> %d %s\n",strlen(position->fen),position->fen);

        // memcpy(position->fen , token,128);



    }

    else if(strcasecmp(token , "moves")==0)
    {
        token = strtok(NULL, " \t\r\n");

        while (token != NULL)
        {
            if (position->moveCount >= MAX_MOVES)
                return false;



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
    // else
    // {
    //     return false;
    // }

    token = strtok(NULL, " \t\r\n");

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

bool parseUCI(char *line, UCI_COMMAND_DATA *command)
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

void* UCI_SERVER(void* arg)
{
    pthread_detach(pthread_self());

    char line[UCI_LINE_SIZE];

    UCI_COMMAND_DATA command;



    while (fgets(line, sizeof(line), stdin))
    {

        if (!parseUCI(line, &command))
        {
            printf("invalid input\n");
            continue;
        }


        switch (command.type)
        {
            case CMD_PRINTBOARD:
                pthread_mutex_lock(&inputLock);
                CMD_CASE=CMD_TYPE_PRINTBOARD;

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

                CMD_CASE=CMD_TYPE_ISREADY;

                pthread_cond_broadcast(&inputCond);
                pthread_mutex_unlock(&inputLock);

                printf("readyok\n");
                fflush(stdout);

                break;



            case CMD_UCINEWGAME:
                pthread_mutex_lock(&inputLock);

                CMD_CASE=CMD_TYPE_ISREADY;

                pthread_cond_broadcast(&inputCond);
                pthread_mutex_unlock(&inputLock);

                printf("readyok\n");
                fflush(stdout);

                break;


            case CMD_POSITION:

                if (command.position.startpos)
                {
                }


                else if (command.position.hasFen)
                {
                    pthread_mutex_lock(&inputLock);

                    CMD_CASE=CMD_TYPE_FENPOSITION;

                    // printf("--> %s\n", command.position.fen);
                    memcpy(retValueOfComputation ,command.position.fen, strlen(command.position.fen)+1);
                    pthread_cond_broadcast(&inputCond);
                    pthread_mutex_unlock(&inputLock);
                }


                for (int i = 0; i < command.position.moveCount; i++)
                {
                    pthread_mutex_lock(&inputLock);
                    memcpy(retValueOfComputation ,command.position.moves[i], strlen(command.position.moves[i])+1);
                    CMD_CASE=CMD_TYPE_MAKEMOVE;
                    pthread_cond_broadcast(&inputCond);
                    pthread_mutex_unlock(&inputLock);

                }
                printf("readyok\n");
                break;

            case CMD_GO:

                //printGoCommand(&command.go);

                if (command.go.perft)
                {
                    pthread_mutex_lock(&inputLock);
                    memcpy(retValueOfComputation, &command.go.perft_level , sizeof(command.go.perft_level));
                    // divide(command.go.depthValue);

                    CMD_CASE=CMD_TYPE_PERFT;
                    pthread_cond_broadcast(&inputCond);
                    pthread_mutex_unlock(&inputLock);

                }
                 /*
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
                return (void*)1;
            default:
                break;
        }
    }
}
