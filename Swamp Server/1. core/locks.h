#pragma once
#include <pthread.h>

////////////////////////////////////////
// -------------------------------------
// ------------ LOCKS ------------------
// -------------------------------------
////////////////////////////////////////

// ------------GLOBAL STATE------------
// ------------ VARIABLE---------------

extern char value[10000];
extern void* retValueOfComputation;

extern pthread_mutex_t inputLock;
extern pthread_cond_t  inputCond ;

typedef enum {
    CMD_TYPE_NOCOMMAND=0,
    CMD_TYPE_ISREADY=1,
    CMD_TYPE_FENPOSITION,
    CMD_TYPE_PERFT,
    CMD_TYPE_PRINTBOARD,
    CMD_TYPE_MAKEMOVE,


} CMD_TYPE;


extern CMD_TYPE CMD_CASE;
