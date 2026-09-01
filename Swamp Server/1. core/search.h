#pragma once
#include <stdint.h>
#include "board.h"
#include "core.h"
#include "evaluation.h"

extern thread_local uint64_t GAME_STATE[24];
extern thread_local uint8_t _chessBoard[64];



uint16_t goSearch();
