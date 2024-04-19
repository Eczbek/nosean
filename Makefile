CC ?= g++
CFLAGS = -std=c++26 -Werror -Wpedantic -Wall -Wextra -Wconversion -Wsign-conversion
BIN_DIR = ./bin

run:
	mkdir -p bin
	${CC} src/main.cpp -o ${BIN_DIR}/main ${CFLAGS} -O2
	${BIN_DIR}/main

debug:
	${CC} src/main.cpp -o ${BIN_DIR}/main ${CFLAGS} -fsanitize=address -fsanitize=undefined -g -Og
	${BIN_DIR}/main
