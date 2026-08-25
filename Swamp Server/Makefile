U_NAME := $(shell uname -s)
CC = gcc
JCC = javac
CFLAGS = -O3 -march=native -flto
SOURCES = Swamp.c ./"2. Board"/board.c ./"4. Search"/search.c ./"7. Engine"/engine.c ./"1. core"/locks.c ./"1. core"/core.c ./"3. MoveGenerator"/onpar.c ./"8. UCI_SERVER"/uciserver.c ./"9. threading/multithread.c"
BENCHMARK_SOUCES = SwampBenchmark.c ./"2. Board"/board.c ./"4. Search"/search.c ./"7. Engine"/engine.c ./"1. core"/locks.c ./"1. core"/core.c ./"3. MoveGenerator"/onpar.c ./"9. threading"/multithread.c
JSOURCES = ./"6. Server"/Server.java 
OUTPUT = Swamp.out



threading ?= NONE

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

ifeq ($(movegenerator),onpar)
	SOURCES += MoveGenerator/onpar.c
endif

engine:
	$(CC) $(CFLAGS) $(SOURCES) -o $(OUTPUT)
	$(JCC) $(JSOURCES)

benchmark:
	$(CC) $(CFLAGS) $(BENCHMARK_SOUCES) -o $(OUTPUT)
	$(JCC) $(JSOURCES)

defaultBuild:
	$(CC) $(CFLAGS) $(SOURCES) -o $(OUTPUT)
	$(JCC) $(JSOURCES)

build:
	$(CC) $(CFLAGS) $(SOURCES) -o $(OUTPUT)
	$(JCC) $(JSOURCES)

run: defaultBuild
	./$(OUTPUT)
	java ../"6. Server"/Server
