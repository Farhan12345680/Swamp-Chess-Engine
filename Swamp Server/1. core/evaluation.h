#pragma once
#include "./multithread.h"
#include "search.h"


extern thread_local uint64_t GAME_STATE[24];
extern thread_local uint8_t  _chessBoard[64];



double evaulateThisPosition();
