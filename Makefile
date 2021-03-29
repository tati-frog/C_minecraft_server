LINKER=-lpthread -luuid
TEST_LINKER=-lcheck -lm -lsubunit -lrt -lpthread -luuid
TEST_OBJECTS=hashtable.o buffer.o mcprotocol.o
FLAGS=-g -I ./src

OBJECTS=handlers.o mcprotocol.o server.o buffer.o hashtable.o

./server.out build: ${OBJECTS}
	cc ${FLAGS} -o server.out ./src/main.c ${OBJECTS} ${LINKER}

./test.out test: ${TEST_OBJECTS}
	cc ${FLAGS} -o test.out ./test/main.c ${TEST_OBJECTS} ${TEST_LINKER}

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
