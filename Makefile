LINKER=-lpthread
FLAGS=-I ./src -o server.out

OBJECTS=main.o handlers.o mcprotocol.o server.o buffer.o hashtable.o

build: ${OBJECTS}
	cc ${FLAGS} ${OBJECTS} ${LINKER}

main.o ./src/main.c:
	cc -I ./src -c ./src/main.c

handlers.o ./src/handlers.c:
	cc -I ./src -c ./src/handlers.c

mcprotocol.o ./src/net/mcprotocol.c:
	cc -I ./src -c ./src/net/mcprotocol.c

server.o ./src/net/server.c:
	cc -I ./src -c ./src/net/server.c

buffer.o ./src/net/utils/buffer.c:
	cc -I ./src -c ./src/net/utils/buffer.c 

hashtable.o /src/net/utils/hashtable.c:
	cc -I ./src -c ./src/net/utils/hashtable.c

clean:
	rm *.o *.out
