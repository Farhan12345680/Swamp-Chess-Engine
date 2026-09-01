#include "locks.h"
#include <pthread.h>
#include "./multithread.h"
#include <stdbool.h>
#include <stdint.h>
char value[10000];
void* retValueOfComputation=(void*)value;







uint16_t moveGeneratedArray[256];
int moveGeneratedArrayIndex=0;
int moveGeneratedLastArrayIndex=0;
pthread_cond_t moveArrayReading=PTHREAD_COND_INITIALIZER;
pthread_mutex_t moveArrayReadingLock=PTHREAD_MUTEX_INITIALIZER;



int ququeSize=0;
commandPoint* head=NULL;
commandPoint* tail=NULL;
pthread_mutex_t inputLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  inputCond = PTHREAD_COND_INITIALIZER;
