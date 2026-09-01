#include "../1. core/engine.h"
#include "../1. core/locks.h"
#include "../1. core/engine.h"
#include "../1. core/search.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>



void* EngineHandler(void* returnT)
{
    pthread_detach(pthread_self());

    while(1)
    {
        pthread_mutex_lock(&inputLock);

        while (!ququeSize)
        {
            pthread_cond_wait(&inputCond, &inputLock);
        }
        commandPoint* temp = head;
        head=head->next;
        ququeSize--;


        switch (temp->CMD_CASE)
        {
            case CMD_TYPE_ISREADY:
            {
                printf("readyok\n");
                fflush(stdout);

                pthread_mutex_unlock(&inputLock);
                break;
            }
            case CMD_TYPE_STARTPOS:
            {
                initializer();

                pthread_mutex_unlock(&inputLock);
                break;
            }
            case CMD_TYPE_FENPOSITION:
            {

                int ret=initializeNewGameFromString(temp->fenString);



                if(ret==-1){
                    printf("Wrong fen\n");
                    initializer();
                }
                pthread_mutex_unlock(&inputLock);
                break;

            }
            case CMD_TYPE_PERFT:
            {
                int level = temp->perftDepth;
                pthread_mutex_unlock(&inputLock);
                struct timespec start, end;
                clock_gettime(CLOCK_MONOTONIC, &start);
                double seconds;
                #ifdef MULTITHREAD
                    divideBulkWithThread(level);
                    clock_gettime(CLOCK_MONOTONIC, &end);

                    seconds =
                        (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1e9;
                    printf("Time: %.9f seconds\n", seconds);
                    break;
                #endif
                divideBulk(level);

                clock_gettime(CLOCK_MONOTONIC, &end);

                seconds = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
                break;
            }
            case CMD_TYPE_MAKEMOVE:
            {

                initializer();
                for(int i =0;i<temp->moveCount; i+=1){
                    uint16_t src  = (uint16_t)((temp->globalPositionMoveOrder[i][0] - 'a') + ((temp->globalPositionMoveOrder[i][1] - '1') * 8));
                    uint16_t dest = (uint16_t)((temp->globalPositionMoveOrder[i][2] - 'a') + ((temp->globalPositionMoveOrder[i][3] - '1') * 8));
                uint16_t move = src | (dest << 6);
                if (temp->globalPositionMoveOrder[i][4] != '\0')
                {
                    uint16_t promo = 0;
                    switch (temp->globalPositionMoveOrder[i][4])
                    {
                        case 'q': promo = 1; break;
                        case 'r': promo = 2; break;
                        case 'b': promo = 3; break;
                        case 'n': promo = 4; break;
                        default:  promo = 0; break;
                    }
                    move |= (promo << 12);
                }
                makeMove(move);

                }


                pthread_mutex_unlock(&inputLock);
                break;
            }
            case CMD_TYPE_GO_SEARCH:
            {
                pthread_mutex_unlock(&inputLock);
                uint16_t move = goSearch();

                if(move==0x0000)
                {

                    printf("bestmove 0000\n");
                    fflush(stdout);
                    break;
                }


                char movearr[6];
                uint16_t src  = move & 63;
                move >>= 6;
                uint16_t dest = move & 63;
                move >>=6;
                uint16_t promotion = move ;
                char promotionPie[5] = {'\0', 'q', 'r', 'b', 'n'};
                movearr[0]=(char)((src%8) + 'a');
                movearr[1]=(char)((src/8) + '1');
                movearr[2]=(char)((dest%8) + 'a');
                movearr[3]=(char)((dest/8) + '1');

                movearr[4] = promotionPie[promotion];
                movearr[5] = '\0';

                printf("bestmove %s\n" , movearr);
                fflush(stdout);
                break;
            }
            case 99:
            {

            }
            case  CMD_TYPE_PRINTBOARD:
            {
                pthread_mutex_unlock(&inputLock);
                printBoard();
                break;
            }
            default:
            {
                break;
            }

        }
        free(temp);
    }
}
