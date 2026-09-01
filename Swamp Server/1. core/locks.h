#pragma once
#include <pthread.h>
#include "./multithread.h"
#include <stdbool.h>
#include <stdint.h>
////////////////////////////////////////
// -------------------------------------
// ------------ LOCKS ------------------
// -------------------------------------
////////////////////////////////////////

// ------------GLOBAL STATE------------
// ------------ VARIABLE---------------

extern char value[10000];
extern void* retValueOfComputation;



typedef enum {
    CMD_TYPE_NOCOMMAND=0,
    CMD_TYPE_ISREADY=1,
    CMD_TYPE_FENPOSITION,
    CMD_TYPE_PERFT,
    CMD_TYPE_PRINTBOARD,
    CMD_TYPE_MAKEMOVE,
    CMD_TYPE_GO_SEARCH,
    CMD_TYPE_STARTPOS,
    CMD_TYPE_STOP

} CMD_TYPE;


typedef struct commandPoint{
    CMD_TYPE CMD_CASE;
    char fenString[1024];
    char globalPositionMoveOrder[256][6];
    int moveCount;
    int perftDepth;
    int wtime;
    int btime;
    int startpos;

    struct commandPoint* next;
} commandPoint;

extern int ququeSize;
extern commandPoint* head;
extern commandPoint* tail;
extern pthread_mutex_t inputLock;
extern pthread_cond_t  inputCond ;






extern char globalPositionMoveOrder[256][6];
extern int positionCount;


// multithreading move generation
extern uint16_t moveGeneratedArray[256];
extern int moveGeneratedArrayIndex;
extern int moveGeneratedLastArrayIndex;
extern pthread_cond_t moveArrayReading;
extern pthread_mutex_t moveArrayReadingLock;


extern uint64_t perftNumber;
extern pthread_cond_t updatePerftCond;
extern pthread_mutex_t updatePerftCondLock;
extern bool updating;
