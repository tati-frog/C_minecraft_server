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

./src/utils/buffer.h ./src/utils/buffer.c buffer.o:
	cc ${FLAGS} -c ./src/utils/buffer.c 

./src/utils/hashtable.h ./src/utils/hashtable.c hashtable.o:
	cc ${FLAGS} -c ./src/utils/hashtable.c

clean:
	rm *.o *.out
