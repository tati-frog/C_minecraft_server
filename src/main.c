#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "net/server.h"
#include "net/mcprotocol.h"

#define SERVER_PORT 25566

void inputHandler(ServerCtx *ctx, int fd)
{
    mc_int packetSize;
    readVarint(fd, &packetSize);

    struct sockaddr_in clientAddr;
    socklen_t addrlen = sizeof(clientAddr);
    memset(&clientAddr, 0, sizeof(clientAddr));

    getpeername(fd, (struct sockaddr*)&clientAddr, &addrlen);
    printf("New packet of %d bytes from %s:%d.\n", packetSize, inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

    char buf[packetSize];
    int bytesReaded;

    recv(fd, buf, packetSize, 0);
    
}

void newConnectionHandler(ServerCtx *ctx, int fd)
{
    printf("New connection!\n");
}

void disconnectionHandler(ServerCtx *ctx, int fd)
{
    printf("Client disconnected\n");
}

int main()
{
    int err;
    ServerCtx *ctx = createServerContext();

    setNewConnectionHandler(ctx, &newConnectionHandler);
    setDisconnectHandler(ctx, &disconnectionHandler);
    setInputDataHandler(ctx, &inputHandler);

    err = bindServer(ctx, 25565, "127.0.0.1");
    if(err == -1)
    {
        perror("Error binding server");
        exit(-1);
    }
    startListening(ctx, 10);

    startEventLoop(ctx);

    for(;;)
    {
        printf("Press q to exit.\n");
        if(getc(stdin) == 'q')
        {
            stopServer(ctx);
            exit(0);
        }
    }
}