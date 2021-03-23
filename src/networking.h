#include <stdlib.h>
#include <poll.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifndef _NETWORKING
#define _NETWORKING

typedef struct
{
    int fd; // Server file descriptor

    struct pollfd pollfdSet[11]; // Set of poll file descriptors, this includes the clients file descriptors and the server socket file descriptor.
    int pollfdSetCount;          // Number of pollfd's in the pollfdSet

    pthread_t eventloopthread;

    void (*handleNewConnectionEvent)(int fd);
    void (*handleDisconnectEvent)(int fd);
    void (*handleInputDataEvent)(int fd);
} ServerCtx;

// Create a server object and start the sockets
ServerCtx *createServerContext();

// Set the handler for new connections.
void setNewConnectionHandler(ServerCtx *ctx, void (*handler)(int));

// Set the handler for new input data.
void setInputDataHandler(ServerCtx *ctx, void (*handler)(int));

// Set the handler for a disconnect event.
void setDisconnectHandler(ServerCtx *ctx, void (*handler)(int));

// Create a socket and bind it.
int bindServer(ServerCtx *ctx, short port, char *addr);

// Start listening on the socket.
int startListening(ServerCtx *ctx, int backlog);

// Start the server eventloop.
int startEventLoop(ServerCtx *ctx);

// Stop eventloop and close sockets
void stopServer(ServerCtx *ctx);

#endif