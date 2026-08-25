#pragma once
#include "./multithread.h"
#include "./locks.h"

#ifdef _WIN32
#include <windows.h>
#elif defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
#endif


extern int THREAD_COUNT ;

void getLogicalThreadCount();


