LINKER=-lpthread
FLAGS=-I ./src -o server.out
FILES=./src/main.c ./src/net/server.c ./src/net/mcprotocol.c ./src/net/utils/buffer.c

build:
	cc ${FLAGS} ${FILES} ${LINKER}

debug:
	cc -g ${FLAGS} ${FILES} ${LINKER}