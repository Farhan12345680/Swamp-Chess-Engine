#pragma once
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include "board.h"
#ifndef __uint64_t
#define __uint64_t uint64_t
#endif
#ifndef __uint8_t
#define __uint8_t uint8_t
#endif


extern GAME_STATE_STRUCT globalGameStruct;


#ifdef MULTITHREAD_ENGINE
extern GAME_STATE_STRUCT globalGameStruct;
extern GAME_STATE_STRUCT[] globalStateArray;
extern int globalStateArrayIndex;
#endif
