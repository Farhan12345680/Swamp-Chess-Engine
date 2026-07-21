#pragma once
#include <stdint.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#define MAX_UCI_QUERY_LENGTH 257

#ifndef __uint64_t
typedef uint64_t __uint64_t;
#endif


__uint64_t generate64BitRandomSlow();


#define SET_BIT_AT(_rankCounter , _currPosition) (((__uint64_t)1)<<(((_rankCounter)-1)*8)<<(7-(_currPosition)))
#define SET_BIT_PIECE(_piece , _rankCounter , _currPosition) \
    do {\
        (_piece) ^= SET_BIT_AT((_rankCounter) , (_currPosition)); \
        (_currPosition)++; \
    } while(0)
