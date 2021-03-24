LINKER=-lpthread
FLAGS=-I ./src -o server.out
FILES=./src/main.c ./src/net/server.c ./src/net/mcprotocol.c ./src/net/utils/dynamicbuffer.c

build:
	cc ${FLAGS} ${FILES} ${LINKER}