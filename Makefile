LINKER=-lpthread
FLAGS=-I ./src -o server.out
FILES=./src/main.c ./src/networking.c

build:
	cc ${FLAGS} ${FILES} ${LINKER}