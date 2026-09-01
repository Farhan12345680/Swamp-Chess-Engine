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


SEARCH = ./"4. Search"/MinMax.c -DGREEDY
EVALUTATION = ./"5. Evaluation"/evaluation1.c
MOVEGENERATOR = ./"3. MoveGenerator"/abovepar.c



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
	SEARCH = ./"4. Search"/greedySearch.c -DGREEDY
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
