#include "core.h"

GAME_STATE_STRUCT globalGameStruct={};


#ifdef MULTITHREAD_ENGINE
GAME_STATE_STRUCT globalGameStruct={};
GAME_STATE_STRUCT[] globalStateArray=[30];
int globalStateArrayIndex =0;
#endif
