#include "locks.h"
#include <pthread.h>


char value[10000];
void* retValueOfComputation=(void*)value;

pthread_mutex_t inputLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  inputCond = PTHREAD_COND_INITIALIZER;



CMD_TYPE CMD_CASE=(CMD_TYPE)0;
