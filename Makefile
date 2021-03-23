LINKER=-lpthread
FLAGS=-I ./src -o server.out
FILES=./src/main.c ./src/net/server.c

build:
	cc ${FLAGS} ${FILES} ${LINKER}