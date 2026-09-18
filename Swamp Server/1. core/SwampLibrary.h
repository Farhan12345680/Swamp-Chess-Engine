#include "engine.h"
#include "locks.h"
#include "engine.h"
#include "search.h"

void init();
void initFromFen(char* fenString);
void doMove(uint16_t move);
char* goSearchNextBestMove(void);
bool isValidMove(uint16_t);
char* getFenStringFromGame();
