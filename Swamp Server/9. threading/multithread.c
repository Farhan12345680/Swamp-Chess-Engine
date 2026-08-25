#include  "../1. core/multithread.h"



int THREAD_COUNT=1;

void getLogicalThreadCount()
{
#ifdef _WIN32
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    THREAD_COUNT=(int)info.dwNumberOfProcessors;

#elif defined(__unix__) || defined(__APPLE__)
    long n = sysconf(_SC_NPROCESSORS_ONLN);
    THREAD_COUNT  = n > 0 ? (int)n : 1;

#endif
}