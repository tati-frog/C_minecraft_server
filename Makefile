LINKER=-lpthread -luuid
TEST_LINKER=-lcheck -lm -lsubunit -lrt -lpthread -luuid
TEST_OBJECTS=hashtable.o buffer.o mcprotocol.o nbt.o
FLAGS=-Wall -g -I ./src

OBJECTS=handlers.o mcprotocol.o server.o buffer.o hashtable.o nbt.o

./server.out build: ${OBJECTS}
	${CC} ${FLAGS} -o server.out ./src/main.c ${OBJECTS} ${LINKER}

./test.out test: ${TEST_OBJECTS}
	${CC} ${FLAGS} -o test.out ./test/main.c ${TEST_OBJECTS} ${TEST_LINKER}

./src/handlers.h ./src/handlers.c handlers.o:
	${CC} ${FLAGS} -c ./src/handlers.c

./src/net/mcprotocol.h ./src/net/mcprotocol.c mcprotocol.o:
	${CC} ${FLAGS} -c ./src/net/mcprotocol.c

./src/net/server.h ./src/net/server.c server.o:
	${CC} ${FLAGS} -c ./src/net/server.c

./src/utils/buffer.h ./src/utils/buffer.c buffer.o:
	${CC} ${FLAGS} -c ./src/utils/buffer.c 

./src/utils/hashtable.h ./src/utils/hashtable.c hashtable.o:
	${CC} ${FLAGS} -c ./src/utils/hashtable.c

./src/nbt/nbt.h ./src/nbt/nbt.c nbt.o:
	${CC} ${FLAGS} -c ./src/nbt/nbt.c

clean:
	rm *.o *.out
