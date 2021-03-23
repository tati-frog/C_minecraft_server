#include <sys/socket.h>
#include <poll.h>
#include <pthread.h>

#ifndef _NETWORKING
#define _NETWORKING

typedef struct ServerCtx
{
    int fd; // Server file descriptor

    struct pollfd *pollfdSet; // Set of poll file descriptors, this includes the clients file descriptors and the server socket file descriptor.
    int pollfdSetCount;          // Number of pollfd's in the pollfdSet

    pthread_t eventloopthread;

    void (*handleNewConnectionEvent)(struct ServerCtx *ctx, int fd);
    void (*handleDisconnectEvent)(struct ServerCtx *ctx, int fd);
    void (*handleInputDataEvent)(struct ServerCtx *ctx, int fd);
} ServerCtx;

// Create a server object and start the sockets
ServerCtx *createServerContext();

// Set the handler for new connections.
void setNewConnectionHandler(ServerCtx *ctx, void (*handler)(ServerCtx*, int));

// Set the handler for new input data.
void setInputDataHandler(ServerCtx *ctx, void (*handler)(ServerCtx*, int));

// Set the handler for a disconnect event.
void setDisconnectHandler(ServerCtx *ctx, void (*handler)(ServerCtx*, int));

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