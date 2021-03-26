#ifndef _NETWORKING
#define _NETWORKING

#include <sys/socket.h>
#include <poll.h>
#include <pthread.h>

#include "./utils/buffer.h"
#include "./utils/hashtable.h"

typedef struct
{
    int fd;
    Buffer *data;
    Buffer *response;

    void *contextData;
} ConnectionCtx;

typedef struct ServerCtx
{
    int fd; // Server file descriptor

    struct pollfd *pollfdSet; // Set of poll file descriptors, this includes the clients file descriptors and the server socket file descriptor.
    int pollfdSetCount;       // Number of pollfd's in the pollfdSet

    HashTable *connectionCtxs; // Hashtable with the context of the connections

    pthread_t eventloopthread;

    void (*handleNewConnectionEvent)(struct ServerCtx *ctx, ConnectionCtx *connectionctx);
    void (*handleDisconnectEvent)(struct ServerCtx *ctx, ConnectionCtx *connectionctx);
    void (*handleInputDataEvent)(struct ServerCtx *ctx, ConnectionCtx *connectionctx);
} ServerCtx;


// Create a server object and start the sockets
ServerCtx *createServerContext();

// Set the handler for new connections.
void setNewConnectionHandler(ServerCtx *ctx, void (*handler)(ServerCtx *ctx, ConnectionCtx* connectionContext));

// Set the handler for new input data.
void setInputDataHandler(ServerCtx *ctx, void (*handler)(ServerCtx *, ConnectionCtx *connectionctx));

// Set the handler for a disconnect event.
void setDisconnectHandler(ServerCtx *ctx, void (*handler)(ServerCtx *ctx, ConnectionCtx* connectionContext));

// Create a socket and bind it.
int bindServer(ServerCtx *ctx, short port, char *addr);

// Start listening on the socket.
int startListening(ServerCtx *ctx, int backlog);

// Start the server eventloop.
int startEventLoop(ServerCtx *ctx);

// Send the buffer data to the socket.
int sendData(int socket, char *buffer, int size);

// Stop eventloop and close sockets
void stopServer(ServerCtx *ctx);

#endif