U_NAME := $(shell uname -s)
CC = gcc
JCC = javac
CFLAGS = -O3 -march=native -flto -std=c17 -D_POSIX_C_SOURCE=200809L
SOURCES = Swamp.c ./"2. Board"/board.c  ./"7. Engine"/engine.c ./"1. core"/locks.c ./"1. core"/core.c ./"8. UCI_SERVER"/uciserver.c ./"9. threading/multithread.c"
BENCHMARK_SOUCES = SwampBenchmark.c ./"2. Board"/board.c ./"7. Engine"/engine.c ./"1. core"/locks.c ./"1. core"/core.c ./"9. threading"/multithread.c
JSOURCES = ./"6. Server"/Server.java
OUTPUT = Swamp.out
DEBUGFLAGS = -g1 -std=c17 -D_POSIX_C_SOURCE=200809L

####################################################
################### FILE SOURCES ###################
####################################################


SEARCH = ./"4. Search"/"Level-4 MinMaxWithAlphaBetaBetter.c" -DGREEDY
EVALUTATION = ./"5. Evaluation"/evaluationBetter.c
MOVEGENERATOR = ./"3. MoveGenerator"/abovepar.c

WASM_SOURCES = ./library/SwampLibrary.c ./"2. Board"/board.c ./"1. core"/core.c $(SEARCH) $(EVALUTATION) $(MOVEGENERATOR)


####################################################
################### INPUT VARIABLES ################
####################################################


threading ?= NONE
movegenerator ?= NONE
search ?= NONE
evaluation ?= NONE


# threading

ifeq ($(threading),y)
		CFLAGS += -DMULTITHREAD
endif


ifeq ($(U_NAME),Linux)
	OUTPUT = Swamp.out
endif

ifeq ($(U_NAME),Darwin)
	OUTPUT = Swamp.out
endif

ifeq ($(findstring MINGW,$(U_NAME)),MINGW)
    OUTPUT = Swamp.exe
endif

ifeq ($(search),greedy)
	SEARCH = ./"4. Search"/Level-1 greedySearch.c -DGREEDY
endif




ifeq ($(movegenerator),above)
	MOVEGENERATOR = ./"3. MoveGenerator"/abovepar.c

endif

engine:
	$(CC) $(CFLAGS) $(SOURCES) $(MOVEGENERATOR) $(SEARCH) $(EVALUTATION) -o $(OUTPUT)
	$(JCC) $(JSOURCES)

benchmark:
	$(CC) $(CFLAGS) $(SOURCES) $(MOVEGENERATOR) $(SEARCH) $(EVALUTATION) -o $(OUTPUT)
	$(JCC) $(JSOURCES)

defaultBuild:
	$(CC) $(CFLAGS) $(SOURCES) $(MOVEGENERATOR) $(SEARCH) $(EVALUTATION) -o $(OUTPUT)
	$(JCC) $(JSOURCES)

build:
	$(CC) $(CFLAGS) $(SOURCES) $(MOVEGENERATOR) $(SEARCH) $(EVALUTATION) -o $(OUTPUT)
	$(JCC) $(JSOURCES)

run: defaultBuild
	./$(OUTPUT)
	java ../"6. Server"/Server

debug:
	$(CC) $(DEBUGFLAGS) $(SOURCES) $(MOVEGENERATOR) $(SEARCH) $(EVALUTATION) -o $(OUTPUT)
	$(JCC) $(JSOURCES)


# emscripten is needed


library1:
	emcc \
		./library/SwampLibrary.c ./"2. Board"/board.c ./"1. core"/core.c ./"4. Search"/"Level-1 greedySearch.c" ./"5. Evaluation"/evaluation2.c ./"3. MoveGenerator"/abovepar.c \
	    -O3 \
	    -sWASM=1 \
	    -sMODULARIZE=1 \
	    -sEXPORT_ES6=1 \
	    -sEXPORTED_FUNCTIONS='["_malloc","_free","_init","_initFromFen","_doMove","_goSearchNextBestMove","_isValidMove","_getFenFromBoard"]' \
	    -sEXPORTED_RUNTIME_METHODS='["UTF8ToString","stringToUTF8","lengthBytesUTF8"]' \
	    -o ../web-gui-interface/js/greedy/swampGreedy.js

library2:
	emcc \
		./library/SwampLibrary.c ./"2. Board"/board.c ./"1. core"/core.c ./"4. Search"/"Level-2 MinMax.c" ./"5. Evaluation"/evaluation2.c ./"3. MoveGenerator"/abovepar.c \
	    -O3 \
	    -sWASM=1 \
	    -sMODULARIZE=1 \
	    -sEXPORT_ES6=1 \
	    -sEXPORTED_FUNCTIONS='["_malloc","_free","_init","_initFromFen","_doMove","_goSearchNextBestMove","_isValidMove","_getFenFromBoard"]' \
	    -sEXPORTED_RUNTIME_METHODS='["UTF8ToString","stringToUTF8","lengthBytesUTF8"]' \
	    -o ../web-gui-interface/js/min_max/swampMinMax.js

library3:
	emcc \
		./library/SwampLibrary.c ./"2. Board"/board.c ./"1. core"/core.c ./"4. Search"/"Level-4 MinMaxWithAlphaBetaBetter.c" ./"5. Evaluation"/evaluation2.c ./"3. MoveGenerator"/abovepar.c \
	    -O3 \
	    -sWASM=1 \
	    -sMODULARIZE=1 \
	    -sEXPORT_ES6=1 \
	    -sEXPORTED_FUNCTIONS='["_malloc","_free","_init","_initFromFen","_doMove","_goSearchNextBestMove","_isValidMove","_getFenFromBoard"]' \
	    -sEXPORTED_RUNTIME_METHODS='["UTF8ToString","stringToUTF8","lengthBytesUTF8"]' \
	    -o ../web-gui-interface/js/alphaBeta/swampMinMaxAlphaBeta.js

wasm: library1 library2 library3
