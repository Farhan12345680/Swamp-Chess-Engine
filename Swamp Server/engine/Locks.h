#pragma once
#include <pthread.h>

////////////////////////////////////////
// -------------------------------------
// ------------ LOCKS ------------------
// -------------------------------------
////////////////////////////////////////

// ------------GLOBAL STATE------------
// ------------ VARIABLE---------------

char value[10000];
void* retValueOfComputation=(void*)value;

pthread_mutex_t inputLock= PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  inputCond = PTHREAD_COND_INITIALIZER;

typedef enum {
    CMD_TYPE_ISREADY=1,
    CMD_TYPE_FENPOSITION,
    CMD_TYPE_PERFT,
    CMD_TYPE_PRINTBOARD,
    CMD_TYPE_MAKEMOVE,


} CMD_TYPE;


CMD_TYPE CMD_CASE=0;




