LINKER=-lpthread -luuid
FLAGS=-g -I ./src

OBJECTS=main.o handlers.o mcprotocol.o server.o buffer.o hashtable.o

build: ${OBJECTS}
	cc ${FLAGS} -o server.out ${OBJECTS} ${LINKER}

./src/main.c main.o:
	cc ${FLAGS} -c ./src/main.c

./src/handlers.h ./src/handlers.c handlers.o:
	cc ${FLAGS} -c ./src/handlers.c

./src/net/mcprotocol.h ./src/net/mcprotocol.c mcprotocol.o:
	cc ${FLAGS} -c ./src/net/mcprotocol.c

./src/net/server.h ./src/net/server.c server.o:
	cc ${FLAGS} -c ./src/net/server.c

./src/net/utils/buffer.h ./src/net/utils/buffer.c buffer.o:
	cc ${FLAGS} -c ./src/net/utils/buffer.c 

./src/net/utils/hashtable.h ./src/net/utils/hashtable.c hashtable.o:
	cc ${FLAGS} -c ./src/net/utils/hashtable.c

clean:
	rm *.o *.out
