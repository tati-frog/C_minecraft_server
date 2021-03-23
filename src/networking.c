#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "networking.h"

void eventloopEntry(ServerCtx *ctx);
void addNewSocket(ServerCtx *ctx, int socket);

ServerCtx *createServerContext()
{
    ServerCtx *ctx = malloc(sizeof(ServerCtx));
    memset(ctx, 0, sizeof(ServerCtx));

    return ctx;
}

void setNewConnectionHandler(ServerCtx *ctx, void(*handler)(int))
{
    ctx->handleNewConnectionEvent = handler;
}

void setInputDataHandler(ServerCtx *ctx, void(*handler)(int))
{
    ctx->handleInputDataEvent = handler;
}

void setDisconnectHandler(ServerCtx *ctx, void(*handler)(int))
{
    ctx->handleDisconnectEvent = handler;
}

int bindServer(ServerCtx *ctx, short port, char *addr)
{
    ctx->fd = socket(AF_INET, SOCK_STREAM, 0);

    addNewSocket(ctx, ctx->fd);

    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr(addr);

    int err = bind(ctx->fd, &serverAddress, sizeof(serverAddress));
    return err;
}

int startListening(ServerCtx *ctx, int backlog)
{
    int err = listen(ctx->fd, backlog);
    return err;
}

int sendData(int socket, char *buffer, int size)
{
    int err = send(socket, buffer, size, 0);
    return err;
}

int startEventLoop(ServerCtx *ctx)
{
    int err = pthread_create(&ctx->eventloopthread, NULL, &eventloopEntry, ctx);
    return err;
}

void stopServer(ServerCtx *ctx)
{
    pthread_cancel(ctx->eventloopthread);

    for (int i = 0; i < ctx->pollfdSetCount; ++i)
    {
        shutdown(ctx->pollfdSet[i].fd, SHUT_RDWR);
        close(ctx->pollfdSet[i].fd);
    }

    free(ctx);
}

void eventloopEntry(ServerCtx *ctx)
{
    for (;;)
    {
        int events = poll(ctx->pollfdSet, ctx->pollfdSetCount, -1);

        for (int i = 0; (i < ctx->pollfdSetCount && events > 0); ++i)
        {
            struct pollfd *currentPollfd = &ctx->pollfdSet[i];

            if ((currentPollfd->revents == POLLIN) && currentPollfd->fd == ctx->fd)
            {
                acceptIncomingConnection(ctx);
                ctx->handleNewConnectionEvent(currentPollfd->fd);
            }
            else if (currentPollfd->revents == POLLIN)
            {
                int availableBytes;
                ioctl(currentPollfd->fd, FIONREAD, &availableBytes);

                if (availableBytes == 0)
                {
                    removeSocket(ctx, currentPollfd->fd);
                    ctx->handleDisconnectEvent(currentPollfd->fd);
                    currentPollfd->revents = 0;
                    events--;
                    continue;
                }

                ctx->handleInputDataEvent(currentPollfd->fd);
            }
            else
            {
                continue;
            }

            currentPollfd->revents = 0;
            events--;
        }
    }
}

void acceptIncomingConnection(ServerCtx *ctx)
{
    int clientSocket = accept(ctx->fd, NULL, NULL);
    addNewSocket(ctx, clientSocket);
}

void addNewSocket(ServerCtx *ctx, int socket)
{
    ctx->pollfdSetCount++;

    struct pollfd socketPollFd;
    memset(&socketPollFd, 0, sizeof(socketPollFd));

    socketPollFd.fd = socket;
    socketPollFd.events = POLLIN;

    ctx->pollfdSet[ctx->pollfdSetCount - 1] = socketPollFd;
}

void removeSocket(ServerCtx *ctx, int socket)
{
    for (int i = 0; i < ctx->pollfdSetCount - 1; ++i)
    {
        if(ctx->pollfdSet[i + 1].fd != socket) continue;

        ctx->pollfdSetCount--;
        struct pollfd tempPollFdSet[10];
        memset(tempPollFdSet, 0, sizeof(tempPollFdSet));

        memcpy(tempPollFdSet, &ctx->pollfdSet[1], i * sizeof(struct pollfd));
        memcpy(&tempPollFdSet[i], &ctx->pollfdSet[i + 2], ctx->pollfdSetCount * sizeof(struct pollfd));

        memset(&ctx->pollfdSet[1], 0, sizeof(tempPollFdSet));
        memcpy(&ctx->pollfdSet[1], tempPollFdSet, sizeof(tempPollFdSet));

        return;
    }
}
