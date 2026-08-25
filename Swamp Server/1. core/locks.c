#include "locks.h"
#include <pthread.h>
#include "./multithread.h"
#include <stdbool.h>
#include <stdint.h>
char value[10000];
void* retValueOfComputation=(void*)value;

pthread_mutex_t inputLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  inputCond = PTHREAD_COND_INITIALIZER;



CMD_TYPE CMD_CASE=(CMD_TYPE)0;


uint16_t moveGeneratedArray[256];
int moveGeneratedArrayIndex=0;
int moveGeneratedLastArrayIndex=0;
pthread_cond_t moveArrayReading=PTHREAD_COND_INITIALIZER;
pthread_mutex_t moveArrayReadingLock=PTHREAD_MUTEX_INITIALIZER;


uint64_t perftNumber=0;
pthread_cond_t updatePerftCond=PTHREAD_COND_INITIALIZER;
pthread_mutex_t updatePerftCondLock=PTHREAD_MUTEX_INITIALIZER;
bool updating = false;