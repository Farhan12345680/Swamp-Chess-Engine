#include "../1. core/engine.h"
#include "../1. core/locks.h"
#include "../1. core/engine.h"



void* EngineHandler(void* returnT)
{
    pthread_detach(pthread_self());

    while(1)
    {
        pthread_mutex_lock(&inputLock);

        while (!CMD_CASE)
        {
            pthread_cond_wait(&inputCond, &inputLock);
        }

        switch (CMD_CASE)
        {
        case CMD_TYPE_ISREADY:{
            initializer(&globalGameStruct);
            int* returnC = (int*)returnT;
            *returnC = 1  ;
            pthread_mutex_unlock(&inputLock);

            break;}
        case CMD_TYPE_FENPOSITION:{

            int ret=initializeNewGameFromString((char*)returnT , &globalGameStruct);

            int* returnC = (int*)returnT;
            *returnC = ret  ;

            if(ret==-1){
                printf("Wrong\n");
                initializer(&globalGameStruct);
            }
            pthread_mutex_unlock(&inputLock);
            break;

        }
        case CMD_TYPE_PERFT:
        {
            int level = ((int *)retValueOfComputation)[0];
            pthread_mutex_unlock(&inputLock);
            struct timespec start, end;
            clock_gettime(CLOCK_MONOTONIC, &start);

            divideBulk(level ,&globalGameStruct);
            clock_gettime(CLOCK_MONOTONIC, &end);

            double seconds =
                (end.tv_sec - start.tv_sec) +
                (end.tv_nsec - start.tv_nsec) / 1e9;
            printf("Time: %.9f seconds\n", seconds);
            break;
        }
        case CMD_TYPE_MAKEMOVE:
        {
            makeMove(((uint16_t*)retValueOfComputation)[0], &globalGameStruct);
            pthread_mutex_unlock(&inputLock);
            break;
        }
        case 99:
        {

        }
        case  CMD_TYPE_PRINTBOARD:
            printBoard(&globalGameStruct);
            pthread_mutex_unlock(&inputLock);

            break;
        default:
            break;
        }
        CMD_CASE=CMD_TYPE_NOCOMMAND;


    }
}
