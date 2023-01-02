.PHONY: all clean run

CXX := g++
CXX_FLAGS := --std=c++17 -O3
CXX_LIBS := -lSDL2 -lSDL2main

SRCS := $(wildcard src/*.cpp)
HDRS := $(wildcard src/*.hpp)
ASMS := $(wildcard src/*.s)
BIN := ./game-of-life

all: ${BIN}

${BIN}: ${SRCS} ${ASMS} ${HDRS}
	${CXX} ${CXX_FLAGS} ${SRCS} ${ASMS} -o $@ ${CXX_LIBS}

run: all
	${BIN}

clean:
	rm ${BIN}
